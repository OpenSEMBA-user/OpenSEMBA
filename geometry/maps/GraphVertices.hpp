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
    map<pair<LayerId,CoordinateId>, GraphBound*> map;
    for (UInt s = 0; s < elems.size(); s++) {
        layId = elems(s)->getLayerId();
        elemPtr = new GraphElem(elems(s), elems(s)->numberOfVertices());
        this->elems_.push_back(elemPtr);
        for (UInt v = 0; v < elems(s)->numberOfVertices(); v++) {
            vertex = elems(s)->getVertex(v);
            if (map.count(make_pair(layId,vertex->getId())) == 0) {
                boundPtr = new GraphBound(vertex);
                this->bounds_.push_back(boundPtr);
                map[make_pair(layId,vertex->getId())] = boundPtr;
            }
            boundPtr = map[make_pair(layId,vertex->getId())];
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
