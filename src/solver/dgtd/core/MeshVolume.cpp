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
#include "MeshVolume.h"

MeshVolume::MeshVolume() {
}

MeshVolume::MeshVolume(
        const MeshUnstructured& uns) {
    this->coords() = *uns.coords().clone();
    this->elems() = *uns.elems().clone();
    this->layers() = *uns.layers().clone();
}

MeshVolume::~MeshVolume() {
}

bool MeshUnstructured::isFloatingCoordinate(const CoordR3* param) const {
    GroupElements<const ElemR> elems =
            GroupElements<ElemR>::getOf<ElemR>();
    for (UInt i = 0; i < elems.size(); i++) {
        for (UInt j = 0; j < elems(i)->numberOfCoordinates(); j++) {
            if (*param == *elems(i)->getV(j)) {
                return false;
            }
        }
    }
    return true;
}

//void MeshVolume::createAndAssignPML(
//        const PMVolumePML::Direction direction[3],
//        const vector<Face>& internalBorder,
//        MeshVolume* mesh) {
//    // Computes bound of PML pointing in this direction.
//    BoxD3 bound = mesh->getBound(internalBorder);
//    pair<CVecR3,CVecR3> pmlBound;
//    for (UInt i = 0; i < 3; i++) {
//        if (direction[i] == PMVolumePML::minus) {
//            pmlBound.second(i) = bound.getMin()(i);
//            pmlBound.first(i) = - numeric_limits<Real>::infinity() ;
//        } else if (direction[i] == PMVolumePML::plus) {
//            pmlBound.first(i) = bound.getMax()(i);
//            pmlBound.first(i) = numeric_limits<Real>::infinity() ;
//        } else {
//            pmlBound.first(i) = bound.getMin()(i);
//            pmlBound.second(i) = bound.getMax()(i);
//        }
//    }
//    // Gets ids of PMLs inside the pml bound. Removes non-tet.
//    vector<UInt> allPMLIds = mesh->getIdsInsideBound(pmlBound);
//    vector<UInt> PMLIds = mesh->getTetIds(allPMLIds);
//    // Creates new PML material for that bound and sets in mesh.
//    if (PMLIds.size() != 0) {
//        UInt lastId = count();
//        pml_.push_back(new PMVolumePML(lastId+1, direction, mesh->getBound(PMLIds)));
//        updatePointers();
//        mesh->setMaterialIds(PMLIds, lastId+1);
//    }
//}
//
//void MeshVolume::detectAndAssignPMLRegions() {
//    if (countPML() == 0) {
//        return;
//    }
//    const UInt pmlId = getPML()->getId();
//    vector<UInt> notPMLId = mesh->elem_.getIdsWithoutMaterialId(pmlId);
//    vector<UInt> internalId = mesh->getTetIds(notPMLId);
//    vector<pair<const Volume*, UInt>> internalBorder =
//            mesh->getInternalBorder(internalId);
//    // Creates PML material stretched towards +x.
//    PMVolumePML::Direction direction[3];
//    for (UInt i = 0; i < PMVolumePML::possibleDirections; i++) {
//        getDirection(direction, i);
//        createAndAssignPML(direction, internalBorder, mesh);
//    }
//}

vector<vector<ElementId>> MeshVolume::getPartitionsIds(
        const UInt nDivisions,
        const vector<pair<ElementId,Int>> idWgt,
        const Real* taskPower) const {
    // Metis v5 manual:
    // [...] take as input the element-node array of the mesh and
    // compute a k-way partitioning for both its elements and its nodes
    // idWgt contains id and weight pairs.
    vector<vector<ElementId>> res;
    res.resize(nDivisions, vector<ElementId>());
    // Accounts for the one partition case.
    if (nDivisions == 1) {
        const UInt nK = elems().sizeOf<VolR>();
        res[0].resize(nK, ElementId(0));
        for (UInt i = 0; i < nK; i++) {
            res[0][i] = (elems())(i)->getId();
        }
        return res;
    }
#ifdef MESH_ALLOW_PARTITIONING
    // Prepares mesh info.
    cout << " - Preparing mesh info... " << flush;
    idx_t ne = elem_.nVolumeElements();
    idx_t *eptr, *eind;
    eptr = new idx_t[ne+1];
    eind = new idx_t[ne*4];
    UInt counter = 0;
    eptr[0] = counter;
    for (Int i = 0; i < (Int) ne; i++) {
        const Tet* vol = elem_.tet[i];
        for (UInt j = 0; j < vol->numberOfVertices(); j++) {
            eind[counter++] = vol->getVertex(j)->id - 1;
        }
        eptr[i+1] = counter;
    }
    cout << "OK" << endl;
    // Relabels ids, needed by quadratic or linearized meshes.
    cout << " - Relabeling... " << flush;
    DynMatrix<Int> id(ne*4,3);
    for (Int i = 0; i < ne*4; i++) {
        id(i,0) = i;
        id(i,1) = eind[i];
        id(i,2) = 0;
    }
    id.sortRows_omp(1,1);
    Int label = 0;
    for (Int i = 1; i < ne*4; i++) {
        if (id(i,1) == id(i-1,1)) {
            id(i,2) = label;
        } else {
            id(i,2) = ++label;
        }
    }
    id.sortRows_omp(0,0);
    for (Int i = 0; i < ne*4; i++) {
        eind[i] = id(i,2);
    }
    idx_t nn = label+1; // Number of vertices.
    cout << "OK" << endl;
    // Copies weights.
    cout << " - Copying weights... " << flush;
    idx_t *vwgt;
    if (idWgt.size() == 0) {
        vwgt = NULL;
    } else {
        vwgt = new idx_t[ne];
        for (Int e = 0; e < ne; e++) {
            vwgt[e] = idWgt[e].second;
        }
    }
    idx_t *vsize = NULL;
    idx_t nparts = nDivisions;
    idx_t objval;
    idx_t *epart;
    epart = new idx_t[ne];
    idx_t *npart;
    npart = new idx_t[nn];
    cout << "OK" << endl;
    // Computes task computational powers.
    real_t *tpwgts = NULL;
    if (taskPower != NULL) {
        tpwgts = new real_t[nDivisions];
        real_t sum = 0.0;
        for (UInt i = 0; i < nDivisions; i++) {
            tpwgts[i] = taskPower[i];
            sum += tpwgts[i];
        }
        assert(abs(sum) - 1.0e-16 < 1.0);
    }
    // METIS options.
    cout << " - Setting Options... " << flush;
    idx_t options[METIS_NOPTIONS];
    Int status;
    status = METIS_SetDefaultOptions(options);
    options[METIS_OPTION_PTYPE] = METIS_PTYPE_KWAY;
    options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_CUT;
    options[METIS_OPTION_SEED] = (idx_t) 0;
    //	options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_VOL;
    // c numbering. Starts from 0.
    options[METIS_OPTION_NUMBERING] = 0;
    cout << "OK" << endl;
    // Calls METIS partition function for meshes.
    idx_t ncommon = 3; // Number of common vertices per element.
    cout << " - Calling Part Mesh Dual... " << flush;
    status = METIS_PartMeshDual(
            &ne, &nn, eptr, eind, vwgt, vsize, &ncommon, &nparts,
            tpwgts, options, &objval, epart, npart);
    if (status != METIS_OK) {
        cerr << endl << "ERROR@Mesh::getPartitionIds()" << endl;
        cerr << endl << "METIS_PartMeshDual fn failed with error: "
                << status << endl;
        exit(MESH_ERROR);
    }
    cout << "OK" << endl;
    // Converts result.
    for (UInt i = 0; i < nDivisions; i++) {
        res[i].reserve(ne);
    }
    for (Int i = 0; i < ne; i++) {
        UInt id = elem_.tet[i]->getId();
        res[epart[i]].push_back(id);
    }
    // Frees memory.
    delete vwgt;
    delete epart;
    delete npart;
    delete eptr;
    delete eind;
    // Returns result.
    return res;
#else
    cerr << endl << "ERROR @ Mesh::getPartition(): "
            << "Mesh partitioning is not allowed." << endl;
    return vector<vector<ElementId>>();
#endif
}

//bool MeshVolume::checkAreaCoherence() const {
//    UInt nElem = elem_.nVolumeElements();
//    bool isOk = true;
//    for (UInt e = 0; e < nElem; e++) {
//        UInt id = elem_.tet[e]->getId();
//        const Tet *local = elem_.getTetPtrToId(id);
//        for (UInt f = 0; f < local->numberOfFaces(); f++) {
//            pair<const Volume*, UInt> localFace(local, f);
//            pair<const Volume*, UInt> neighFace;
//            neighFace = getNeighConnection(localFace);
//            const Volume* neigh = neighFace.first;
//            UInt f2 = neighFace.second;
//            Real diff =
//                    local->getAreaOfFace(f) - neigh->getAreaOfFace(f2);
//            if (abs(diff) > areaDiffTolerance) {
//                cerr << endl << "Id: " << local->getId()
//				                 << " Face: " << f
//				                 << " Diff: " << diff << endl;
//                isOk = false;
//            }
//        }
//    }
//    if (!isOk) {
//        cerr << endl << "ERROR @ checkAreaCoherence()" << endl;
//        cerr << endl << "Check above id-face pairs." << endl;
//    }
//    return isOk;
//}
//
