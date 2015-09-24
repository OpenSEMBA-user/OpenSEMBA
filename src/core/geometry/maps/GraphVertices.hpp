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
 * GraphVertices.hpp
 *
 *  Created on: 15/4/2015
 *      Author: Daniel
 */

#include "GraphVertices.h"

template<class ELEM, class BOUND>
GraphVertices<ELEM,BOUND>::GraphVertices() {

}

template<class ELEM, class BOUND>
GraphVertices<ELEM,BOUND>::GraphVertices(
        const Group<const GraphVertices<ELEM,BOUND>::Elem>&  elems,
        const Group<const GraphVertices<ELEM,BOUND>::Bound>& bounds) {
    init(elems, bounds);
}

template<class ELEM, class BOUND>
GraphVertices<ELEM,BOUND>::~GraphVertices() {

}

template<class ELEM, class BOUND>
GraphVertices<ELEM,BOUND>& GraphVertices<ELEM,BOUND>::init(
        const Group<const GraphVertices<ELEM,BOUND>::Elem>& elems,
        const Group<const GraphVertices<ELEM,BOUND>::Bound>& bounds) {
    LayerId layId;
    const Bound* vertex;
    GraphElem*   elemPtr;
    GraphBound*  boundPtr;
    map<CoordinateId, GraphBound*> map;
    for (UInt s = 0; s < elems.size(); s++) {
        layId = elems(s)->getLayerId();
        elemPtr = new GraphElem(elems(s), elems(s)->numberOfVertices());
        this->elems_.push_back(elemPtr);
        for (UInt v = 0; v < elems(s)->numberOfVertices(); v++) {
            vertex = elems(s)->getVertex(v);
            if (map.count(vertex->getId()) == 0) {
                boundPtr = new GraphBound(vertex);
                this->bounds_.push_back(boundPtr);
                map[vertex->getId()] = boundPtr;
            }
            boundPtr = map[vertex->getId()];
            elemPtr->setBound(v, boundPtr);
            boundPtr->addBound(elemPtr);
        }
    }
    map.clear();
    for (UInt i = 0; i < this->elems_.size(); i++) {
        this->elems_[i]->constructNeighbors();
    }
    for (UInt i = 0; i < this->bounds_.size(); i++) {
        this->bounds_[i]->constructNeighbors();
    }
    return *this;
}

template<class ELEM, class BOUND>
void GraphVertices<ELEM,BOUND>::splitBound(UInt i) {
    vector<GraphElem*> oldAdj = this->bounds_[i]->getBounds();
    if (oldAdj.empty()) {
        return;
    }
    this->bounds_[i]->setBounds(oldAdj[0]);
    this->bounds_[i]->getBound(0)->constructNeighbors();
    this->bounds_[i]->constructNeighbors();
    for (UInt j = 1; j < oldAdj.size(); j++) {
        this->bounds_.push_back(new GraphBound(*this->bounds_[i]));
        this->bounds_.back()->setBounds(oldAdj[j]);
        this->bounds_.back()->getBound(0)->constructNeighbors();
        this->bounds_.back()->constructNeighbors();
    }
}
