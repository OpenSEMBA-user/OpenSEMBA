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
 * GraphVertices.h
 *
 *  Created on: 15/4/2015
 *      Author: Daniel
 */

#ifndef COMMON_GEOMETRY_MAPS_GRAPHVERTICES_H_
#define COMMON_GEOMETRY_MAPS_GRAPHVERTICES_H_

#include "../graphs/GraphBase.h"

template<class ELEM, class BOUND>
class GraphVertices : public GraphBase<ELEM, BOUND> {
public:
    typedef ELEM  Elem;
    typedef BOUND Bound;
    typedef GraphElement<Elem,Bound> GraphElem;
    typedef GraphElement<Bound,Elem> GraphBound;

    GraphVertices();
    GraphVertices(const Group<const Elem>&  elems,
                  const Group<const Bound>& bounds);
    virtual ~GraphVertices();

    GraphVertices<Elem,Bound>& init(const Group<const Elem>&  elems,
                                    const Group<const Bound>& bounds);

    void splitBound(UInt i);
};

#include "../graphs/GraphVertices.hpp"

#endif /* COMMON_GEOMETRY_MAPS_GRAPHVERTICES_H_ */
