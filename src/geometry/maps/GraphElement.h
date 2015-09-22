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
 * GraphElement.h
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#ifndef COMMON_GEOMETRY_MAPS_GRAPHELEMENT_H_
#define COMMON_GEOMETRY_MAPS_GRAPHELEMENT_H_


#include <utility>
#include <vector>
using namespace std;

#include "../elements/Element.h"

template<class ELEM, class BOUND>
class GraphElement {
public:
    typedef ELEM  Elem;
    typedef BOUND Bound;
    typedef GraphElement<Elem,Bound> GraphElem;
    typedef GraphElement<Bound,Elem> GraphBound;

    GraphElement(const Elem* elem, const UInt& numBounds = 0);
    GraphElement(const GraphElement&);

    bool visited() const { return vis_;  }
    void markVisited  () { vis_ =  true; }
    void unmarkVisited() { vis_ = false; }

    const Elem* elem() const { return elem_; }

    UInt numBounds() const { return bounds_.size(); }
    vector<const GraphBound*> getBounds() const;
    vector<GraphBound*>       getBounds()    { return bounds_;    }
    const GraphBound* getBound(UInt i) const { return bounds_[i]; }
    GraphBound*       getBound(UInt i)       { return bounds_[i]; }
    void setBounds(GraphBound* bound);
    void setBounds(vector<GraphBound*> bounds) { bounds_ = bounds; }
    void setBound(UInt i, GraphBound* bound)   { bounds_[i] = bound;       }
    void addBound(GraphBound* bound)           { bounds_.push_back(bound); }

    UInt numNeighbors() const { return neighbors_.size(); }
    const GraphElem* getNeighbor(UInt i) const { return neighbors_[i]; }
    GraphElem*       getNeighbor(UInt i)       { return neighbors_[i]; }

    UInt numBoundNeighbors(UInt i) const { return boundNeighbors_[i].size(); }
    const GraphElem* getBoundNeighbor(UInt i, UInt j) const;
    GraphElem*       getBoundNeighbor(UInt i, UInt j);

    void constructNeighbors();

    void printInfo() const;

private:
    bool vis_;

    const Elem* elem_;
    vector<GraphBound*>        bounds_;
    vector<GraphElem*>         neighbors_;
    vector<vector<GraphElem*>> boundNeighbors_;
};

#include "GraphElement.hpp"

#endif /* COMMON_GEOMETRY_MAPS_GRAPHELEMENT_H_ */
