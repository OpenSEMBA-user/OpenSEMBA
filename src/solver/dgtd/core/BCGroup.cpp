// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

/*
 * BCGroup.cpp
 *
 *  Created on: Jul 8, 2013
 *      Author: luis
 */

#ifndef BCGROUP_H_
#include "BCGroup.h"
#endif

BCGroup::BCGroup(
        const SmbData* smb,
        const CellGroup* cells) {
    const MeshVolume* mesh = smb->mesh;
    const EMSourceGroup* em = smb->emSources;
    const PMGroup* pm = smb->pMGroup;
    buildEMSourceBC(*mesh, *em, *cells);
    buildPhysicalModelBC(*mesh, *pm, *cells);
    removeOverlapped();
    check();
}

BCGroup& BCGroup::operator=(const BCGroup &rhs) {
    if (this == &rhs) {
        return *this;
    }
    embc = rhs.embc;
    pmbc = rhs.pmbc;
    sibc = rhs.sibc;
    return *this;
}

void BCGroup::buildEMSourceBC(
        const MeshVolume& mesh,
        const EMSourceGroup& em,
        const CellGroup& cells) {
    vector<Face> border;
    for (uint i = 0; i < em.size(); i++) {
        const EMSourceBase* source = em(i);
        border = mesh.getInternalBorder(source->elems());
        for (uint j = 0; j < border.size(); j++) {
            const CellTet<ORDER_N>* auxCell = cells.getPtrToCell(border[j].first);
            uint face = border[j].second;
            EMSourceBC auxBC(auxCell, face, source);
            embc.push_back(auxBC);
        }
    }
}

void BCGroup::buildPhysicalModelBC(
        const MeshVolume& mesh,
        const PMGroup& pm,
        const CellGroup& cells) {
    GroupElements<Surf> surf = mesh.elems().getOf<Surf>();
    for (uint i = 0; i < surf.size(); i++) {
        const TriMap* sMap = &mesh.map.tri[i];
        // Finds elements neighbouring surface.
        if (surf->getMatId() != 0) {
            const Condition* mat = pm.getPMWithId(surf->getMatId());
            pair<const Tet*, uint> tFace = sMap->getInnerFace();
            const CellTet<ORDER_N>* cell
            = cells.getPtrToCell(tFace.first);
            uint face = tFace.second;
            if (!mat->isSurfaceImpedance()) {
                pmbc.push_back(PhysicalModelBC(cell, face, mat));
                if (!sMap->isBoundary()) {
                    tFace = sMap->getOuterFace();
                    cell = cells.getPtrToCell(tFace.first);
                    face = tFace.second;
                    pmbc.push_back(PhysicalModelBC(cell, face, mat));
                }
            } else {
                assert(!sMap->isBoundary());
                pair<const Tet*, uint> neigh;
                neigh = mesh.getNeighConnection(tFace);
                const CellTet<ORDER_N>* nCell;
                nCell = cells.getPtrToCell(neigh.first);
                const uint nFace = neigh.second;
                if (cell->isLocalSide(face, surf)) {
                    sibc.push_back(
                            SurfaceImpedanceBC(cell,face,nCell,nFace,mat));
                } else {
                    sibc.push_back(
                            SurfaceImpedanceBC(nCell,nFace,cell,face,mat));
                }
            }
        }
    }
}

void BCGroup::removeOverlapped() {
    // Hierarchy in boundary conditions.
    // PEC, PMC, SMA, SIBC > EM Source
    // Builds separated lists.
    vector<BoundaryCondition*> pec, pmc, sma, sibc, em;
    for (uint i = 0; i < pmbc.size(); i++) {
        if(pmbc[i].isSMA()) {
            sma.push_back(&pmbc[i]);
        } else if (pmbc[i].isPEC()) {
            pec.push_back(&pmbc[i]);
        } else if (pmbc[i].isPMC()) {
            pmc.push_back(&pmbc[i]);
        } else if (pmbc[i].isSurfaceImpedance()) {
            sibc.push_back(&pmbc[i]);
        }
    }
    em.reserve(embc.size());
    for (uint i = 0; i < embc.size(); i++) {
        em.push_back(&embc[i]);
    }
    // Removes bc overlapping PEC. After this, PEC is cleaned.
    pmc = removeCommons(pmc, pec);
    sma = removeCommons(sma, pec);
    em = removeCommons(em, pec);
    // Cleans PMC
    sma = removeCommons(sma, pmc);
    em = removeCommons(em, pmc);
    // Cleans SMA
    em = removeCommons(em, sma);
    // Cleans SIBC
    em = removeCommons(em, sibc);
    // Rebuilds lists.
    vector<PhysicalModelBC> auxPMBC;
    auxPMBC.reserve(
            pec.size() + pmc.size() + sma.size() + sibc.size());
    for (uint i = 0; i < pec.size(); i++) {
        auxPMBC.push_back(PhysicalModelBC(*pec[i]));
    }
    for (uint i = 0; i < pmc.size(); i++) {
        auxPMBC.push_back(PhysicalModelBC(*pmc[i]));
    }
    for (uint i = 0; i < sma.size(); i++) {
        auxPMBC.push_back(PhysicalModelBC(*sma[i]));
    }
    for (uint i = 0; i < sibc.size(); i++) {
        auxPMBC.push_back(PhysicalModelBC(*sibc[i]));
    }
    pmbc = auxPMBC;
    vector<EMSourceBC> auxEMBC;
    auxEMBC.reserve(em.size());
    for (uint i = 0; i < em.size(); i++) {
        auxEMBC.push_back(EMSourceBC(*em[i]));
    }
    embc = auxEMBC;
}

vector<BoundaryCondition*> BCGroup::removeCommons(
        const vector<BoundaryCondition*>& low,
        const vector<BoundaryCondition*>& high) const {
    vector<BoundaryCondition*> res;
    res.reserve(low.size());
    for (uint i = 0; i < low.size(); i++) {
        bool isPresentInHigh = false;
        for (uint j = 0; j < high.size(); j++) {
            if (low[i]->hasSameBoundary(*high[j])) {
                isPresentInHigh = true;
                break;
            }
        }
        if (!isPresentInHigh) {
            res.push_back(low[i]);
        }
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToPEC() const {
    vector<const BoundaryCondition*> res;
    for (uint i = 0; i < pmbc.size(); i++) {
        if (pmbc[i].isPEC()) {
            const BoundaryCondition* ptr = &pmbc[i];
            res.push_back(ptr);
        }
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToPMC() const {
    vector<const BoundaryCondition*> res;
    for (uint i = 0; i < pmbc.size(); i++) {
        if (pmbc[i].isPMC()) {
            const BoundaryCondition* ptr = &pmbc[i];
            res.push_back(ptr);
        }
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToSMA() const {
    vector<const BoundaryCondition*> res;
    for (uint i = 0; i < pmbc.size(); i++) {
        if (pmbc[i].isSMA()) {
            const BoundaryCondition* ptr = &pmbc[i];
            res.push_back(ptr);
        }
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToSIBC() const {
    vector<const BoundaryCondition*> res;
    for (uint i = 0; i < sibc.size(); i++) {
        const BoundaryCondition* ptr = &sibc[i];
        res.push_back(ptr);
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToEMSourceBC() const {
    vector<const BoundaryCondition*> res;
    for (uint i = 0; i < embc.size(); i++) {
        const BoundaryCondition* ptr = &embc[i];
        res.push_back(ptr);
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToBC(const EMSourceBase* pw) const {
    vector<const BoundaryCondition*> res;
    res.reserve(embc.size());
    for (uint i = 0; i < embc.size(); i++) {
        if (pw == embc[i].condition) {
            res.push_back(&embc[i]);
        }
    }
    return res;
}

void BCGroup::printInfo() const {
    cout << "--- BCGroup info ---" << endl;
    cout << "EM Source BCs: " << embc.size() << endl;
    cout << "Physical Model BCs: " << pmbc.size() << endl;
    cout << "Surface Impedance BCs: " << sibc.size() << endl;
}

void BCGroup::check() const {
    checkEMSourcesAreSetInVacuum();
    //assert(checkOverlapping());
}

vector<const BoundaryCondition*> BCGroup::getPtrsToBCWithMatId(
        const uint id) const {
    vector<const BoundaryCondition*> res;
    res.reserve(pmbc.size());
    for (uint i = 0; i < pmbc.size(); i++) {
        if (pmbc[i].condition->getId() == id) {
            res.push_back(&pmbc[i]);
        }
    }
    res.reserve(sibc.size());
    for (uint i = 0; i < sibc.size(); i++) {
        if (sibc[i].condition->getId() == id) {
            res.push_back(&sibc[i]);
        }
    }
    return res;
}

void BCGroup::checkEMSourcesAreSetInVacuum() const {
    uint nBC = embc.size();
    for (uint i = 0; i < nBC; i++) {
        if (!embc[i].cell->material->isVacuum()) {
            cerr << "ERROR @ Boundary Conditions."   << endl;
            cerr << "ElectromagneticSource BC has been" << endl;
            cerr << "defined over a not vacuum cell." << endl;
            exit(BC_ERROR);
        }
    }
}

bool BCGroup::checkOverlapping() const {
    bool repeated = false;
    // Check repeated elements in embc and pmbc
    for (uint i = 0; i < embc.size(); i++) {
        for (uint j = 0; j < pmbc.size(); j++) {
            if (embc[i].cell == pmbc[j].cell
                    && embc[i].face == pmbc[j].face) {
                repeated = true;
                break;
            }
        }
    }
    if (repeated) {
        cerr << "ERROR@BCGroup::checkRepeated()" << endl;
        cerr << "Overlapping boundary conditions detected." << endl;
    }
    return repeated;
}
