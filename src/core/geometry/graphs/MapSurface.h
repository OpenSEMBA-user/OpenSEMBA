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
 * MapSurface.h
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MAPSURFACE_H_
#define SRC_COMMON_GEOMETRY_MAPSURFACE_H_

#include "../graphs/Map.h"

class MapSurface: public Map {
public:
   MapSurface();
   virtual ~MapSurface();
   MapSurface(
         const Triangle* localSurf, pair<const Tetrahedron*, const Tetrahedron*>);
   MapSurface&
   operator=(const MapSurface& rhs);
   UInt
   getLocalId() const {return local->getId().toUInt();}
   UInt
    getVolToF(UInt f) const;
   const Tetrahedron*
    getVol(UInt f) const {return vol[f];}
   void
    reassignPointers(const GroupElements<const Elem>& nEG);
   bool
    isBoundary() const;
   void
    printInfo() const;
private:
   const Triangle* local;
   const Tetrahedron* vol[2];
   UInt volToF[2];
};

#endif /* SRC_COMMON_GEOMETRY_MAPSURFACE_H_ */
