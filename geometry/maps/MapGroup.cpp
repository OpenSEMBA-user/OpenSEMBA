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
   static const unsigned int faces = 4;
   static const unsigned int nVert = 3;
   vector<const Tet*> tet = eG.getVectorOf<Tet>();
   unsigned int nK = tet.size();
   unsigned int nList = nK * faces;
   DynMatrix<unsigned int> fList(nList, 2 + nVert);
   for (unsigned int k = 0; k < nK; k++) {
      ElementId id = tet[k]->getId();
      const Tet* aux = eG.getPtrToId(id)->castTo<Tet>();
      for (unsigned int f = 0; f < faces; f++) {
         unsigned int row = k * faces + f;
         fList(row, 0) = id;
         fList(row, 1) = f;
         unsigned int ordered[nVert];
         aux->getOrderedSideVerticesId(ordered, f);
         for (unsigned int i = 0; i < nVert; i++) {
            fList(row, i + 2) = ordered[i];
         }
      }
   }
   // Sorts according to the coordinates id. Pairing matching faces.
   // If there is not a pair, then that element is not connected with
   // other remaining connected with itself.
   fList.sortRows(2,4);
   // Initializes connectivities matrix.
   DynMatrix<unsigned int> etoe(nK, faces);
   DynMatrix<unsigned int> etof(nK, faces);
   for (unsigned int k = 0; k < nK; k++) {
      for (unsigned int f = 0; f < faces; f++) {
         etoe(k,f) = k;
         etof(k,f) = f;
      }
   }
   // Inserts information of pairings into the connectivity matrix.
   for (unsigned int i = 0; i < (nList-1); i++) {
      bool matches;
      matches = fList(i,2) == fList(i+1,2);
      matches &= fList(i,3) == fList(i+1,3);
      matches &= fList(i,4) == fList(i+1,4);
      if (matches) {
         unsigned int k1 = fList(i,0);
         unsigned int f1 = fList(i,1);
         unsigned int k2 = fList(i+1,0);
         unsigned int f2 = fList(i+1,1);
         etoe(k1,f1) = k2;
         etof(k1,f1) = f2;
         etoe(k2,f2) = k1;
         etof(k2,f2) = f1;
      }
   }
   // Generates tetrahedron maps.
   for (unsigned int k = 0; k < nK; k++) {
      const Tet *local = eG.getPtrToId(tet[k]->getId())->castTo<Tet>();
      const Tet *neigh[4];
      unsigned int neighFaces[4];
      for (unsigned int j = 0; j < 4; j++) {
         neigh[j] = eG.getPtrToId(ElementId(etoe(k,j)))->castTo<Tet>();
         neighFaces[j] = etof(k,j);
      }
      pair<uint, MapVolume*>
       aux(local->getId(), new MapVolume(local, neigh, neighFaces));
      tet_.insert(aux);
   }
   // Now uses the generated ordered fList to build the triangle maps.
   vector<const Tri*> tri = eG.getVectorOf<Tri>();
   const unsigned int nS = tri.size();
   for (unsigned int s = 0; s < nS; s++) {
      ElementId id = tri[s]->getId();
      const Tri* local = eG.getPtrToId(id)->castTo<Tri>();
      pair<const Tet*, const Tet*> neigh;
      unsigned int ordered[nVert];
      local->getOrderedVerticesId(ordered);
      unsigned int i = fList.findFirstOcurrenceInColumns(ordered, 2, 3);
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
      pair<uint, MapSurface*> aux(local->getId(), new MapSurface(local, neigh));
      tri_.insert(aux);
   }
}

void
MapGroup::reassignPointers(const ElementsGroup<>& newEG) {
   {
      map<uint,MapVolume*>::iterator it;
      for (it=tet_.begin(); it != tet_.end(); ++it) {
         it->second->reassignPointers(newEG);
      }
   }
   {
      map<uint,MapSurface*>::iterator it;
      for (it=tri_.begin(); it != tri_.end(); ++it) {
         it->second->reassignPointers(newEG);
      }
   }
}

const Tet* MapGroup::getNeighbour(const uint id, const uint face) const {
   return tet_.find(id)->second->getVol(face);
}

uint MapGroup::getVolToF(const uint id, const uint face) const {
   return tet_.find(id)->second->getVolToF(face);
}


pair<const Volume*, unsigned int>
MapGroup::getInnerFace(const uint id) const {
   map<uint,MapSurface*>::const_iterator surf = tri_.find(id);
   assert(surf != tri_.end());
   const Tet* vol = surf->second->getVol(0);
   const uint face = surf->second->getVolToF(0);
   return pair<const Tet*, uint>(vol, face);
}

pair<const Volume*, unsigned int>
MapGroup::getOuterFace(const uint id) const {
   const Volume* vol = tri_.find(id)->second->getVol(1);
   const uint face = tri_.find(id)->second->getVolToF(1);
   return pair<const Volume*, uint>(vol, face);
}

pair<const Tet*, uint>
MapGroup::getNeighConnection(
      const uint id,
      const uint face) const {
   pair<const Tet*, uint> res;
   res.first = getNeighbour(id, face);
   res.second = getVolToF(id, face);
   return res;
}

bool MapGroup::isBoundary(const uint id) const {
   return tri_.find(id)->second->isBoundary();
}

bool MapGroup::isDomainBoundary(const uint id, const uint f) const {
   return (getNeighbour(id,f)->getId() == id);
}

bool MapGroup::isDomainBoundary(pair<const Volume*, uint> boundary) const {
   return isDomainBoundary(boundary.first->getId(), boundary.second);
}

//pair<const Tet*, const Tet*>
//MapGroup::getNeighbours(const Tri* param) const {
//   // PURPOSE:
//   // Returns a pointers to Tets that is adjacent to the SurfaceElement.
//   bool isNeighbour = false;
//   unsigned int vPresent;
//   unsigned int nK = tet_.count()size();
//   for (unsigned int i = 0; i < nK; i++) {
//      const Tet* cTet = tet_[i].local;
//      for (unsigned int f = 0; f < 4; f++) {
//         // Checks current face.
//         vPresent = 0;
//         for (unsigned int j = 0; j < 3; j++) {
//            for (unsigned int k = 0; k < 3; k++) {
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
//   unsigned int count = 0;
//   for (unsigned int e = 0; e < tet_.size(); e++) {
//      const Tet* e1 = tet_[e].local;
//      for (unsigned int f = 0; f < 4; f++) {
//         const Tet* e2 = tet_[e].vol[f];
//         unsigned int f2 = tet_[e].volToF[f];
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
