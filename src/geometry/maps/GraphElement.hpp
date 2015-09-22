/*
 * GraphElement.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#include "GraphElement.h"

template<class ELEM, class BOUND>
GraphElement<ELEM,BOUND>::GraphElement(const ELEM* elem, const UInt& numBound) {
    vis_  = false;
    elem_ = elem;
    bounds_.resize(numBound);
}

template<class ELEM, class BOUND>
GraphElement<ELEM,BOUND>::GraphElement(const GraphElement& rhs) {
    vis_       = rhs.vis_;
    elem_      = rhs.elem_;
    bounds_    = rhs.bounds_;
    neighbors_ = rhs.neighbors_;
    boundNeighbors_ = rhs.boundNeighbors_;
}

template<class ELEM, class BOUND>
vector<const typename GraphElement<ELEM,BOUND>::GraphBound*>
    GraphElement<ELEM,BOUND>::getBounds() const {
    return vector<const GraphBound*>(bounds_.begin(), bounds_.end());
}

template<class ELEM, class BOUND>
void GraphElement<ELEM,BOUND>::setBounds(
    GraphElement<ELEM,BOUND>::GraphBound* bound) {
    bounds_.resize(1);
    bounds_[0] = bound;
}

template<class ELEM, class BOUND>
const typename GraphElement<ELEM,BOUND>::GraphElem*
    GraphElement<ELEM,BOUND>::getBoundNeighbor(UInt i, UInt j) const {
    return boundNeighbors_[i][j];
}

template<class ELEM, class BOUND>
typename GraphElement<ELEM,BOUND>::GraphElem*
    GraphElement<ELEM,BOUND>::getBoundNeighbor(UInt i, UInt j) {
    return boundNeighbors_[i][j];
}

template<class ELEM, class BOUND>
void GraphElement<ELEM,BOUND>::constructNeighbors() {
    boundNeighbors_.clear();
    boundNeighbors_.resize(bounds_.size());
    for (UInt i = 0; i < bounds_.size(); i++) {
        for (UInt j = 0; j < bounds_[i]->numBounds(); j++) {
            if (bounds_[i]->getBound(j)->elem()->getId() != elem_->getId()) {
                boundNeighbors_[i].push_back(bounds_[i]->getBound(j));
            }
        }
    }
    set<GraphElem*> neighbors;
    for (UInt i = 0; i < boundNeighbors_.size(); i++) {
        for (UInt j = 0; j < boundNeighbors_[i].size(); j++) {
            neighbors.insert(boundNeighbors_[i][j]);
        }
    }
    neighbors_ = vector<GraphElem*>(neighbors.begin(), neighbors.end());
}

template<class ELEM, class BOUND>
void GraphElement<ELEM,BOUND>::printInfo() const {
    cout << "--- GraphElement Info ---" << endl;
    elem_->printInfo();
    cout << "Bounds:";
    for (UInt i = 0; i < bounds_.size(); ++i) {
        cout << " " << bounds_[i]->elem()->getId();
    }
    cout << endl;
    if (!neighbors_.empty()) {
        cout << "Neighbors:";
        for (UInt i = 0; i < neighbors_.size(); ++i) {
            cout << " " << neighbors_[i]->elem()->getId();
        }
        cout << endl;
        cout << "Bound Neighbors:" << endl;
        for (UInt i = 0; i < boundNeighbors_.size(); ++i) {
            cout << bounds_[i]->elem()->getId() << ":";
            for (UInt j = 0; j < boundNeighbors_[i].size(); ++j) {
                cout << " " << boundNeighbors_[i][j]->elem()->getId();
            }
            cout << endl;
        }
    }
}
