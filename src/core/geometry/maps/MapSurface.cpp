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
      const Triangle* localSurf,
      pair<const Tetrahedron*, const Tetrahedron*> neighbours) {
   local = localSurf;
   const Tetrahedron* n1 = neighbours.first;
   const Tetrahedron* n2 = neighbours.second;
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
MapSurface::reassignPointers(const GroupElements<const Elem>& nEG) {
    local = nEG.getId(local->getId())->castTo<Triangle>();
    for (UInt i = 0; i < 2; i++) {
        vol[i] = nEG.getId(vol[i]->getId())->castTo<Tetrahedron>();
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
