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
 * MapGroup.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#include "MapGroup.h"

MapGroup::ErrorHangingNode::ErrorHangingNode()
:   Error("Map Group: A surface has a hanging node") {

}

MapGroup::ErrorHangingNode::~ErrorHangingNode() throw () {

}

MapGroup::MapGroup() {
}

MapGroup::~MapGroup() {
}

MapGroup::MapGroup(const GroupCoordinates<const Coord>& cG,
        const GroupElements   <const Elem> & eG) {
    // Builds a list with all tetrahedron faces.
    static const UInt faces = 4;
    static const UInt nVert = 3;
    GroupElements<const Tetrahedron> tet = eG.getOf<Tetrahedron>();
    UInt nK = tet.size();
    UInt nList = nK * faces;
    DynMatrix<UInt> fList(nList, 2 + nVert);
    for (UInt k = 0; k < nK; k++) {
        const Tetrahedron* aux = tet(k);
        ElementId id = aux->getId();
        for (UInt f = 0; f < faces; f++) {
            UInt row = k * faces + f;
            fList(row, 0) = id.toUInt();
            fList(row, 1) = f;
            vector<CoordinateId> ordered(nVert);
            ordered = ElementBase::getIds(aux->getSideVertices(f));
            ordered = ElementBase::ascendingIdOrder(ordered);
            for (UInt i = 0; i < nVert; i++) {
                fList(row, i + 2) = ordered[i].toUInt();
            }
        }
    }
    // Sorts according to the coordinates id. Pairing matching faces.
    // If there is not a pair, then that element is not connected with
    // other remaining connected with itself.
    fList.sortRows(2,4);
    // Initializes connectivities matrix.
    DynMatrix<UInt> etoe(nK, faces);
    DynMatrix<UInt> etof(nK, faces);
    for (UInt k = 0; k < nK; k++) {
        for (UInt f = 0; f < faces; f++) {
            etoe(k,f) = k;
            etof(k,f) = f;
        }
    }
    // Inserts information of pairings into the connectivity matrix.
    for (UInt i = 0; i < (nList-1) && (nList != 0); i++) {
        bool matches;
        matches = fList(i,2) == fList(i+1,2);
        matches &= fList(i,3) == fList(i+1,3);
        matches &= fList(i,4) == fList(i+1,4);
        if (matches) {
            UInt k1 = fList(i,0);
            UInt f1 = fList(i,1);
            UInt k2 = fList(i+1,0);
            UInt f2 = fList(i+1,1);
            etoe(k1,f1) = k2;
            etof(k1,f1) = f2;
            etoe(k2,f2) = k1;
            etof(k2,f2) = f1;
        }
    }
    // Generates tetrahedron maps.
    for (UInt k = 0; k < nK; k++) {
        const Tetrahedron *local = tet(k);
        const Tetrahedron *neigh[4];
        UInt neighFaces[4];
        for (UInt j = 0; j < 4; j++) {
            neigh[j] = eG.getId(ElementId(etoe(k,j)))->castTo<Tetrahedron>();
            neighFaces[j] = etof(k,j);
        }
        pair<UInt, MapVolume*> aux(local->getId().toUInt(),
                new MapVolume(local, neigh, neighFaces));
        tet_.insert(aux);
    }
    // Now uses the generated ordered fList to build the triangle maps.
    GroupElements<const Triangle> tri = eG.getOf<Triangle>();
    const UInt nS = tri.size();
    for (UInt s = 0; s < nS; s++) {
        const Triangle* local = tri(s);
        ElementId id = local->getId();
        pair<const Tetrahedron*, const Tetrahedron*> neigh;
        vector<CoordinateId> ordered(nVert);
        ordered = ElementBase::getIds(local->getVertices());
        ordered = ElementBase::ascendingIdOrder(ordered);
        vector<UInt> orderedInt(ordered.size());
        for (UInt i = 0; i < orderedInt.size(); i++) {
            orderedInt[i] = ordered[i].toUInt();
        }
        UInt i = fList.findFirstOcurrenceInColumns(&orderedInt[0], 2, 3);
        if (i == fList.nRows()) {
            throw ErrorHangingNode();
        }
        //
        bool matches;
        if (i < nList - 1 && nList != 0) {
            matches = fList(i,2) == fList(i+1,2);
            matches &= fList(i,3) == fList(i+1,3);
            matches &= fList(i,4) == fList(i+1,4);
        } else {
            matches = false;
        }
        if (matches) {
            neigh.first = eG.getId(ElementId(fList(i,0)))->castTo<Tetrahedron>();
            neigh.second = eG.getId(ElementId(fList(i+1,0)))->castTo<Tetrahedron>();
        } else {
            neigh.first = eG.getId(ElementId(fList(i,0)))->castTo<Tetrahedron>();
            neigh.second = neigh.first;
        }
        pair<UInt, MapSurface*> aux(local->getId().toUInt(),
                new MapSurface(local, neigh));
        tri_.insert(aux);
    }
}

void MapGroup::reassignPointers(const GroupElements<const Elem>& newEG) {
    {
        map<UInt,MapVolume*>::iterator it;
        for (it=tet_.begin(); it != tet_.end(); ++it) {
            it->second->reassignPointers(newEG);
        }
    }
    {
        map<UInt,MapSurface*>::iterator it;
        for (it=tri_.begin(); it != tri_.end(); ++it) {
            it->second->reassignPointers(newEG);
        }
    }
}

const Tetrahedron* MapGroup::getNeighbour(const ElementId id, const UInt face) const {
    return tet_.find(id)->second->getVol(face);
}

UInt MapGroup::getVolToF(const ElementId id, const UInt face) const {
    return tet_.find(id)->second->getVolToF(face);
}


Face MapGroup::getInnerFace(const ElementId id) const {
    map<UInt,MapSurface*>::const_iterator surf = tri_.find(id);
    assert(surf != tri_.end());
    const Tetrahedron* vol = surf->second->getVol(0);
    const UInt face = surf->second->getVolToF(0);
    return pair<const Tetrahedron*, UInt>(vol, face);
}

Face MapGroup::getOuterFace(const ElementId id) const {
    const VolR* vol = tri_.find(id)->second->getVol(1);
    const UInt face = tri_.find(id)->second->getVolToF(1);
    return pair<const VolR*, UInt>(vol, face);
}

Face MapGroup::getNeighConnection(
        const Face& face) const {
    Face res;
    res.first = getNeighbour(face.first->getId(), face.second);
    res.second = getVolToF(face.first->getId(), face.second);
    return res;
}

bool MapGroup::isBoundary(const ElementId  id) const {
    return tri_.find(id)->second->isBoundary();
}

bool MapGroup::isDomainBoundary(const ElementId id, const UInt f) const {
    return (getNeighbour(id,f)->getId() == ElementId(id));
}

bool MapGroup::isDomainBoundary(Face boundary) const {
    return isDomainBoundary(boundary.first->getId(), boundary.second);
}

//pair<const Tet*, const Tet*>
//MapGroup::getNeighbours(const Tri* param) const {
//   // PURPOSE:
//   // Returns a pointers to Tets that is adjacent to the SurfaceElement.
//   bool isNeighbour = false;
//   UInt vPresent;
//   UInt nK = tet_.count()size();
//   for (UInt i = 0; i < nK; i++) {
//      const Tet* cTet = tet_[i].local;
//      for (UInt f = 0; f < 4; f++) {
//         // Checks current face.
//         vPresent = 0;
//         for (UInt j = 0; j < 3; j++) {
//            for (UInt k = 0; k < 3; k++) {
//               if (*param->getVertex(j) == *cTet->getSideVertex(f, k)) {
//                  vPresent++;
//               }
//            }
//            if (vPresent == 3) {
//               isNeighbour = true;
//            }
//         }
//         // If all nodes are in present face.
//         if (isNeighbour) {
//            const Tet* cTet2 = tet_[i].vol[f];
//            pair<const Tet*, const Tet*> res(cTet, cTet2);
//            return res;
//         }
//      }
//   }
//   //
//   cerr << endl << "ERROR @ Mesh::getNeighbours(...)" << endl;
//   param->printInfo();
//   cerr << endl << " Tri " << param->getId() << " is disconnected.";
//}

//bool
//MapGroup::checkReciprocity() const {
//   bool res = true;
//   UInt count = 0;
//   for (UInt e = 0; e < tet_.size(); e++) {
//      const Tet* e1 = tet_[e].local;
//      for (UInt f = 0; f < 4; f++) {
//         const Tet* e2 = tet_[e].vol[f];
//         UInt f2 = tet_[e].volToF[f];
//         const Map* m2 = getPtrToLocalId(e2->getId());
//         if (m2->getVol(f2) != e1) {
//            if (!res) {
//               cerr << endl << "ERROR @ MapGroup::check()"<< endl;
//               cerr << endl << "Map fail reciprocity check:"    << endl;
//            }
//            cerr << endl << "Elem " << e1->getId() << ", face " << f << endl;
//            cerr << endl << "connects with element "
//                  << e2->getId() << " face " << f2 << endl;
//            cerr << endl << "But that face connects with element "
//                  << m2->getVol(f2)->getId() << endl;
//            count++;
//            if (count == 10) {
//               cerr << endl << "And maybe more..." << endl;
//            }
//            res = false;
//         }
//      }
//   }
//   return res;
//}
