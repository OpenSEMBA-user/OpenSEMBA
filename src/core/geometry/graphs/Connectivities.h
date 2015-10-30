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

#ifndef SRC_COMMON_GEOMETRY_GRAPHS_CONNECTIVITIES_H_
#define SRC_COMMON_GEOMETRY_GRAPHS_CONNECTIVITIES_H_

#include <vector>
#include <limits.h>

using namespace std;

#include "base/error/Error.h"

typedef pair<const VolR*, UInt> Face;

class Connectivities {
public:
    typedef GraphElement<ElemR,CoordR3> GraphElem;

    Connectivities();
    Connectivities(
            const GroupCoordinates<const Coord>& cG,
            const GroupElements  <const Elem> & eG);
    virtual ~Connectivities();

    // Returns face of volume matching given face.
    Face getNeighFace(const Face& face) const;

    // Returns surface matching face.
    const SurfR* getNeighSurf(const Face& face) const;

    // Returns face of volume element connecting the surface.
    Face getInnerFace(const SurfR& surf) const;
    Face getOuterFace(const SurfR& surf) const;

    bool isDomainBoundary(const SurfR& surf) const;
    bool isDomainBoundary(Face boundary) const;

private:
    GraphVertices<ElemR,CoordR3> graph_;
    map<ElementId, const GraphElem*> index_;

    Face getMatchingFace_(
            const GraphElem* local,
            const vector<const Coord*> localSideV) const;
};


#endif
