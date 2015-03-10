/*
 * MapSurface.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#include "MapSurface.h"

MapSurface::MapSurface() {
   local = NULL;
   for (UInt i = 0; i < 2; i++) {
      vol[i] = NULL;
   }
}

MapSurface::~MapSurface() {
   local = NULL;
   for (UInt i = 0; i < 2; i++) {
      vol[i] = NULL;
   }
}

MapSurface::MapSurface(
      const Tri* localSurf,
      pair<const Tet*, const Tet*> neighbours) {
   local = localSurf;
   const Tet* n1 = neighbours.first;
   const Tet* n2 = neighbours.second;
   UInt f = n1->getFaceNumber(localSurf);
   if (n1->isLocalFace(f, *local)) {
      vol[0] = n1;
      volToF[0] = f;
      vol[1] = n2;
      volToF[1] = n2->getFaceNumber(local);
   } else {
      vol[0] = n2;
      volToF[0] = n2->getFaceNumber(local);
      vol[1] = n1;
      volToF[1] = f;
   }
}

MapSurface&
MapSurface::operator=(const MapSurface& rhs) {
   if (this == &rhs)
      return *this;
   local = rhs.local;
   for (UInt i = 0; i < 2; i++) {
      vol[i] = rhs.vol[i];
      volToF[i] = rhs.volToF[i];
   }
   return *this;
}

void
MapSurface::reassignPointers(const ElementsGroup<>& nEG) {
    local = nEG.getPtrToId(local->getId())->castTo<Tri>();
    for (UInt i = 0; i < 2; i++) {
        vol[i] = nEG.getPtrToId(vol[i]->getId())->castTo<Tet>();
    }
}

bool
MapSurface::isBoundary() const {
   return (vol[0] == vol[1] && volToF[0] == volToF[1]);
}

UInt MapSurface::getVolToF(UInt f) const {
   return volToF[f];
}

void
MapSurface::printInfo() const {
   cout << "--- MapSurface Info ---" << endl;
   cout << "Local Id: " << local->getId() << endl;
   cout << "Neighbours Tet Ids: ";
   for (UInt i = 0; i < 2; i++) {
      cout << vol[i]->getId() << " ";
   }
   cout << endl;
   cout << "Through faces: ";
   for (UInt i = 0; i < 2; i++) {
      cout << volToF[i] << " ";
   }
   cout << endl;
}
