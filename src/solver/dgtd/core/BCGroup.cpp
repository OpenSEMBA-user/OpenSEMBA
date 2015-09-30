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

#include "BCGroup.h"

BCGroup::BCGroup(
        const SmbData& smb,
        const CellGroup& cells,
        const MapGroup& map) {
    const MeshVolume* mesh = smb.mesh;
    const EMSourceGroup* em = smb.emSources;
    const PMGroup* pm = smb.pMGroup;
    buildEMSourceBC(*mesh, *em, cells);
    buildPhysicalModelBC(*mesh, *pm, cells, map);
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
    for (UInt i = 0; i < em.size(); i++) {
        const EMSourceBase* source = em(i);
        border = mesh.getInternalBorder(source->elems());
        for (UInt j = 0; j < border.size(); j++) {
            const CellTet<ORDER_N>* auxCell = cells.getPtrToCell(border[j].first);
            UInt face = border[j].second;
            EMSourceBC auxBC(auxCell, face, source);
            embc.push_back(auxBC);
        }
    }
}

void BCGroup::buildPhysicalModelBC(
        const MeshVolume& mesh,
        const PMGroup& pm,
        const CellGroup& cells,
        const MapGroup& map) {
    GroupElements<SurfR> surf = mesh.elems().getOf<SurfR>();
    for (UInt i = 0; i < surf.size(); i++) {
        if (surf(i)->getMatId() != 0) {
            const PhysicalModel* mat = pm.getId(surf(i)->getMatId());
            Face tFace = map.getInnerFace(surf(i)->getId());
            const CellTet<ORDER_N>* cell = cells.getPtrToCell(tFace.first);
            const UInt face = tFace.second;
            if (!mat->is<PMSurfaceSIBC>()) {
                pmbc.push_back(PhysicalModelBC(cell, face, mat));
                if (!map.isDomainBoundary(tFace)) {
                    tFace = map.getOuterFace(surf(i)->getId());
                    cell = cells.getPtrToCell(tFace.first);
                    face = tFace.second;
                    pmbc.push_back(PhysicalModelBC(cell, face, mat));
                }
            } else {
                assert(!sMap->isBoundary());
                Face neigh = map.getNeighConnection(tFace);
                const CellTet<ORDER_N>* nCell = cells.getPtrToCell(neigh.first);
                const UInt nFace = neigh.second;
                if (cell->isLocalSide(face, surf(i))) {
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
    for (UInt i = 0; i < pmbc.size(); i++) {
        if(pmbc[i].getCondition()->is<PMSMA>()) {
            sma.push_back(&pmbc[i]);
        } else if (pmbc[i].getCondition()->is<PMPEC>()) {
            pec.push_back(&pmbc[i]);
        } else if (pmbc[i].getCondition()->is<PMPMC>()) {
            pmc.push_back(&pmbc[i]);
        } else if (pmbc[i].getCondition()->is<PMSurfaceSIBC>()) {
            sibc.push_back(&pmbc[i]);
        }
    }
    em.reserve(embc.size());
    for (UInt i = 0; i < embc.size(); i++) {
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
    for (UInt i = 0; i < pec.size(); i++) {
        auxPMBC.push_back(PhysicalModelBC(*pec[i]));
    }
    for (UInt i = 0; i < pmc.size(); i++) {
        auxPMBC.push_back(PhysicalModelBC(*pmc[i]));
    }
    for (UInt i = 0; i < sma.size(); i++) {
        auxPMBC.push_back(PhysicalModelBC(*sma[i]));
    }
    for (UInt i = 0; i < sibc.size(); i++) {
        auxPMBC.push_back(PhysicalModelBC(*sibc[i]));
    }
    pmbc = auxPMBC;
    vector<EMSourceBC> auxEMBC;
    auxEMBC.reserve(em.size());
    for (UInt i = 0; i < em.size(); i++) {
        auxEMBC.push_back(EMSourceBC(*em[i]));
    }
    embc = auxEMBC;
}

vector<BoundaryCondition*> BCGroup::removeCommons(
        const vector<BoundaryCondition*>& low,
        const vector<BoundaryCondition*>& high) const {
    vector<BoundaryCondition*> res;
    res.reserve(low.size());
    for (UInt i = 0; i < low.size(); i++) {
        bool isPresentInHigh = false;
        for (UInt j = 0; j < high.size(); j++) {
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
    for (UInt i = 0; i < pmbc.size(); i++) {
        if (pmbc[i].getCondition()->is<PMPEC>()) {
            const BoundaryCondition* ptr = &pmbc[i];
            res.push_back(ptr);
        }
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToPMC() const {
    vector<const BoundaryCondition*> res;
    for (UInt i = 0; i < pmbc.size(); i++) {
        if (pmbc[i].getCondition()->is<PMPMC>()) {
            const BoundaryCondition* ptr = &pmbc[i];
            res.push_back(ptr);
        }
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToSMA() const {
    vector<const BoundaryCondition*> res;
    for (UInt i = 0; i < pmbc.size(); i++) {
        if (pmbc[i].getCondition()->is<PMSMA>()) {
            const BoundaryCondition* ptr = &pmbc[i];
            res.push_back(ptr);
        }
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToSIBC() const {
    vector<const BoundaryCondition*> res;
    for (UInt i = 0; i < sibc.size(); i++) {
        const BoundaryCondition* ptr = &sibc[i];
        res.push_back(ptr);
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToEMSourceBC() const {
    vector<const BoundaryCondition*> res;
    for (UInt i = 0; i < embc.size(); i++) {
        const BoundaryCondition* ptr = &embc[i];
        res.push_back(ptr);
    }
    return res;
}

vector<const BoundaryCondition*> BCGroup::getPtrsToBC(const EMSourceBase* pw) const {
    vector<const BoundaryCondition*> res;
    res.reserve(embc.size());
    for (UInt i = 0; i < embc.size(); i++) {
        if (pw == embc[i].getCondition()) {
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
        const UInt id) const {
    vector<const BoundaryCondition*> res;
    res.reserve(pmbc.size());
    for (UInt i = 0; i < pmbc.size(); i++) {
        if (pmbc[i].getCondition()->getId() == id) {
            res.push_back(&pmbc[i]);
        }
    }
    res.reserve(sibc.size());
    for (UInt i = 0; i < sibc.size(); i++) {
        if (sibc[i].getCondition()->getId() == id) {
            res.push_back(&sibc[i]);
        }
    }
    return res;
}

void BCGroup::checkEMSourcesAreSetInVacuum() const {
    UInt nBC = embc.size();
    for (UInt i = 0; i < nBC; i++) {
        if (!embc[i].cell_->material->isVacuum()) {
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
    for (UInt i = 0; i < embc.size(); i++) {
        for (UInt j = 0; j < pmbc.size(); j++) {
            if (embc[i].cell_ == pmbc[j].cell_
                    && embc[i].face_ == pmbc[j].face_) {
                repeated = true;
                break;
            }
        }
    }
    if (repeated) {
        throw Error("Overlapping boundary conditions detected.");
    }
    return repeated;
}
