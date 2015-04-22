/*
 * MapVolume.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#include "MapVolume.h"

MapVolume::MapVolume() {
   local = NULL;
   for (UInt i = 0; i < 4; i++) {
      vol[i] = NULL;
      volToF[i] = 0;
   }
}

MapVolume::~MapVolume() {
   local = NULL;
   for (UInt i = 0; i < 4; i++) {
      vol[i] = NULL;
      volToF[i] = 0;
   }
}

MapVolume::MapVolume(
      const Tet* local_,
      const Tet* neighboor[4],
      const UInt neighboorFaces[4]) {
   local = local_;
   for (UInt i = 0; i < 4; i++) {
      vol[i] = neighboor[i];
      volToF[i] = neighboorFaces[i];
   }
}

const Tet*
MapVolume::getLocalTet() const {
   return local;
}

MapVolume&
MapVolume::operator=(const MapVolume& rhs) {
   if (this == &rhs) {
      return *this;
   }
   local = rhs.local;
   for (UInt i = 0; i < 4; i++) {
      vol[i] = rhs.vol[i];
      volToF[i] = rhs.volToF[i];
   }
   return *this;
}

void
MapVolume::reassignPointers(const GroupElements<const Elem>& nEG) {
   local = nEG.get(local->getId())->castTo<Tet>();
   for (UInt i = 0; i < 4; i++) {
      vol[i] = nEG.get(vol[i]->getId())->castTo<Tet>();
   }
}

bool
MapVolume::faceIsBoundary(UInt f) const {
   return (local == vol[f]);
}

void
MapVolume::printInfo() const {
   cout << "--- MapVolume Info ---" << endl;
   cout << "Local Id: " << local->getId() << endl;
   cout << "Neighbours Ids: ";
   for (UInt i = 0; i < local->numberOfFaces(); i++) {
      cout << vol[i]->getId() << " ";
   }
   cout << endl;
   cout << "Through faces: ";
   for (UInt i = 0; i < local->numberOfFaces(); i++) {
      cout << volToF[i] << " ";
   }
   cout << endl;
}
