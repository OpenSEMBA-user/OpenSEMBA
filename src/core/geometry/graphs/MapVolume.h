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
 * MapVolume.h
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MAPVOLUME_H_
#define SRC_COMMON_GEOMETRY_MAPVOLUME_H_

#include "../graphs/Map.h"

class MapVolume: public Map {
public:
   MapVolume();
   virtual ~MapVolume();
   MapVolume(
    const Tetrahedron* local,
    const Tetrahedron* neighboor[4],
    const UInt neighboorFaces[4]);
   MapVolume&
    operator=(const MapVolume& rhs);
   UInt
    getLocalId() const {return local->getId().toUInt();}
   const Tetrahedron*
    getLocalTet() const;
   UInt
    getVolToF(UInt f) const {return volToF[f];}
   void
    reassignPointers(const GroupElements<const Elem>& nEG);
   const Tetrahedron*
    getVol(UInt f) const {return vol[f];}
   bool
    faceIsBoundary(UInt f) const;
   void
    printInfo() const;
private:
   const Tetrahedron* local;
   const Tetrahedron* vol[4];
   UInt volToF[4];
};

#endif /* SRC_COMMON_GEOMETRY_MAPVOLUME_H_ */
