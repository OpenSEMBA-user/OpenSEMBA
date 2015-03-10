/*
 * MapGroup.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#include "MapGroup.h"

MapGroup::MapGroup() {
}

MapGroup::~MapGroup() {
}

void
MapGroup::build(const CoordinateGroup<>& cG, const ElementsGroup<>& eG) {
   // Builds a list with all tetrahedron faces.
   static const UInt faces = 4;
   static const UInt nVert = 3;
   vector<const Tet*> tet = eG.getVectorOf<Tet>();
   UInt nK = tet.size();
   UInt nList = nK * faces;
   DynMatrix<UInt> fList(nList, 2 + nVert);
   for (UInt k = 0; k < nK; k++) {
      ElementId id = tet[k]->getId();
      const Tet* aux = eG.getPtrToId(id)->castTo<Tet>();
      for (UInt f = 0; f < faces; f++) {
         UInt row = k * faces + f;
         fList(row, 0) = id;
         fList(row, 1) = f;
         UInt ordered[nVert];
         aux->getOrderedSideVerticesId(ordered, f);
         for (UInt i = 0; i < nVert; i++) {
            fList(row, i + 2) = ordered[i];
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
   for (UInt i = 0; i < (nList-1); i++) {
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
      const Tet *local = eG.getPtrToId(tet[k]->getId())->castTo<Tet>();
      const Tet *neigh[4];
      UInt neighFaces[4];
      for (UInt j = 0; j < 4; j++) {
         neigh[j] = eG.getPtrToId(ElementId(etoe(k,j)))->castTo<Tet>();
         neighFaces[j] = etof(k,j);
      }
      pair<UInt, MapVolume*>
       aux(local->getId(), new MapVolume(local, neigh, neighFaces));
      tet_.insert(aux);
   }
   // Now uses the generated ordered fList to build the triangle maps.
   vector<const Tri*> tri = eG.getVectorOf<Tri>();
   const UInt nS = tri.size();
   for (UInt s = 0; s < nS; s++) {
      ElementId id = tri[s]->getId();
      const Tri* local = eG.getPtrToId(id)->castTo<Tri>();
      pair<const Tet*, const Tet*> neigh;
      UInt ordered[nVert];
      local->getOrderedVerticesId(ordered);
      UInt i = fList.findFirstOcurrenceInColumns(ordered, 2, 3);
      if (i == fList.nRows()) {
         cerr << "Faces list." << endl;
         fList.printInfo();
         cerr << "Surface:" << endl;
         local->printInfo();
         cerr << "ERROR @ building maps: "
               << "A surface has a hanging node." << endl;
      }
      //
      bool matches;
      if (i < nList - 1) {
         matches = fList(i,2) == fList(i+1,2);
         matches &= fList(i,3) == fList(i+1,3);
         matches &= fList(i,4) == fList(i+1,4);
      } else {
         matches = false;
      }
      if (matches) {
         neigh.first = eG.getPtrToId(ElementId(fList(i,0)))->castTo<Tet>();
         neigh.second = eG.getPtrToId(ElementId(fList(i+1,0)))->castTo<Tet>();
      } else {
         neigh.first = eG.getPtrToId(ElementId(fList(i,0)))->castTo<Tet>();
         neigh.second = neigh.first;
      }
      pair<UInt, MapSurface*> aux(local->getId(), new MapSurface(local, neigh));
      tri_.insert(aux);
   }
}

void
MapGroup::reassignPointers(const ElementsGroup<>& newEG) {
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

const Tet* MapGroup::getNeighbour(const UInt id, const UInt face) const {
   return tet_.find(id)->second->getVol(face);
}

UInt MapGroup::getVolToF(const UInt id, const UInt face) const {
   return tet_.find(id)->second->getVolToF(face);
}


pair<const Volume*, UInt>
MapGroup::getInnerFace(const UInt id) const {
   map<UInt,MapSurface*>::const_iterator surf = tri_.find(id);
   assert(surf != tri_.end());
   const Tet* vol = surf->second->getVol(0);
   const UInt face = surf->second->getVolToF(0);
   return pair<const Tet*, UInt>(vol, face);
}

pair<const Volume*, UInt>
MapGroup::getOuterFace(const UInt id) const {
   const Volume* vol = tri_.find(id)->second->getVol(1);
   const UInt face = tri_.find(id)->second->getVolToF(1);
   return pair<const Volume*, UInt>(vol, face);
}

pair<const Tet*, UInt>
MapGroup::getNeighConnection(
      const UInt id,
      const UInt face) const {
   pair<const Tet*, UInt> res;
   res.first = getNeighbour(id, face);
   res.second = getVolToF(id, face);
   return res;
}

bool MapGroup::isBoundary(const UInt id) const {
   return tri_.find(id)->second->isBoundary();
}

bool MapGroup::isDomainBoundary(const UInt id, const UInt f) const {
   return (getNeighbour(id,f)->getId() == id);
}

bool MapGroup::isDomainBoundary(pair<const Volume*, UInt> boundary) const {
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
//   cerr << "ERROR @ Mesh::getNeighbours(...)" << endl;
//   param->printInfo();
//   cerr << " Tri " << param->getId() << " is disconnected.";
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
//               cerr << "ERROR @ MapGroup::check()"<< endl;
//               cerr << "Map fail reciprocity check:"    << endl;
//            }
//            cerr << "Elem " << e1->getId() << ", face " << f << endl;
//            cerr << "connects with element "
//                  << e2->getId() << " face " << f2 << endl;
//            cerr << "But that face connects with element "
//                  << m2->getVol(f2)->getId() << endl;
//            count++;
//            if (count == 10) {
//               cerr << "And maybe more..." << endl;
//            }
//            res = false;
//         }
//      }
//   }
//   return res;
//}
