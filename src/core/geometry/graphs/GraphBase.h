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
 * GraphBase.h
 *
 *  Created on: 17/5/2015
 *      Author: Daniel
 */

#ifndef COMMON_GEOMETRY_MAPS_GRAPHBASE_H_
#define COMMON_GEOMETRY_MAPS_GRAPHBASE_H_

#include <queue>
#include <map>
#include <utility>
#include <vector>
using namespace std;

#include "base/group/Group.h"

#include "../graphs/GraphElement.h"

template<class ELEM, class BOUND>
class GraphBase {
public:
    typedef ELEM  Elem;
    typedef BOUND Bound;
    typedef GraphElement<Elem,Bound> GraphElem;
    typedef GraphElement<Bound,Elem> GraphBound;

    GraphBase();
    virtual ~GraphBase();

    virtual GraphBase& init(const Group<const Elem>& elems) = 0;

    UInt numElems () const { return elems_.size();  }
    UInt numBounds() const { return bounds_.size(); }

    const GraphElem*  elem (UInt i) const { return elems_ [i]; }
    GraphElem*        elem (UInt i)       { return elems_ [i]; }
    const GraphBound* bound(UInt i) const { return bounds_[i]; }
    GraphBound*       bound(UInt i)       { return bounds_[i]; }

    void resetVisited();
    vector<vector<const Elem*>> getConnectedComponents();

    void printInfo() const;

protected:
    vector<GraphElem* > elems_;
    vector<GraphBound*> bounds_;
};

#include "../graphs/GraphBase.hpp"

#endif /* COMMON_GEOMETRY_MAPS_GRAPHBASE_H_ */
