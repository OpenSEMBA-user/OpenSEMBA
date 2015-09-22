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
      const Tetrahedron* local_,
      const Tetrahedron* neighboor[4],
      const UInt neighboorFaces[4]) {
   local = local_;
   for (UInt i = 0; i < 4; i++) {
      vol[i] = neighboor[i];
      volToF[i] = neighboorFaces[i];
   }
}

const Tetrahedron*
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
   local = nEG.getId(local->getId())->castTo<Tetrahedron>();
   for (UInt i = 0; i < 4; i++) {
      vol[i] = nEG.getId(vol[i]->getId())->castTo<Tetrahedron>();
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
