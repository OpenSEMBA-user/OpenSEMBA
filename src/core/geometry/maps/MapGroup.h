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
 * MapGroup.h
 *
 *  Created on: Jan 23, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MAPGROUP_H_
#define SRC_COMMON_GEOMETRY_MAPGROUP_H_

#include <vector>
#include <limits.h>

using namespace std;

#include "base/error/Error.h"

#include "MapSurface.h"
#include "MapVolume.h"

typedef pair<const VolR*, UInt> Face;

class MapGroup {
public:
    class ErrorHangingNode : public Error {
    public:
        ErrorHangingNode();
        virtual ~ErrorHangingNode() throw();
    };

    MapGroup();
    MapGroup(const GroupCoordinates<const Coord>& cG,
             const GroupElements  <const Elem> & eG);
    virtual ~MapGroup();
    void reassignPointers(const GroupElements<const Elem>& nEG);
    const Tetrahedron* getNeighbour(const UInt id, const UInt face) const;
    UInt getVolToF(const UInt id, const UInt face) const;
    Face getNeighConnection(const UInt id, const UInt face) const;
    Face getInnerFace(const UInt id) const;
    Face getOuterFace(const UInt id) const;
    bool isBoundary(const UInt id) const;
    bool isDomainBoundary(const UInt id, const UInt f) const;
    bool isDomainBoundary(Face boundary) const;
private:
    map<UInt, MapSurface*> tri_;
    map<UInt, MapVolume*> tet_;
};


#endif
