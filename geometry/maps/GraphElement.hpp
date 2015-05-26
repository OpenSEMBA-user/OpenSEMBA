/*
 * GraphElement.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#include "GraphElement.h"

template<class ELEM, class BOUND>
GraphElement<ELEM,BOUND>::GraphElement(const ELEM* elem, const UInt& numBound) {
    elem_ = elem;
    bounds_.resize(numBound);
}

template<class ELEM, class BOUND>
GraphElement<ELEM,BOUND>::GraphElement(const GraphElement& rhs) {
    elem_      = rhs.elem_;
    bounds_    = rhs.bounds_;
    neighbors_ = rhs.neighbors_;
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
UInt  GraphElement<ELEM,BOUND>::numNeighbors() const {
    UInt res = 0;
    for (UInt i = 0; i < neighbors_.size(); i++) {
        res += neighbors_[i].size();
    }
    return res;
}

template<class ELEM, class BOUND>
const typename GraphElement<ELEM,BOUND>::GraphElem*
    GraphElement<ELEM,BOUND>::getNeighbor(UInt j) const {

    for (UInt i = 0; i < neighbors_.size(); i++) {
        if (j < neighbors_[i].size()) {
            return neighbors_[i][j];
        } else {
            j -= neighbors_[i].size();
        }
    }
    return NULL;
}

template<class ELEM, class BOUND>
typename GraphElement<ELEM,BOUND>::GraphElem*
    GraphElement<ELEM,BOUND>::getNeighbor(UInt j) {

    for (UInt i = 0; i < neighbors_.size(); i++) {
        if (j < neighbors_[i].size()) {
            return neighbors_[i][j];
        } else {
            j -= neighbors_[i].size();
        }
    }
    return NULL;
}

template<class ELEM, class BOUND>
const typename GraphElement<ELEM,BOUND>::GraphElem*
    GraphElement<ELEM,BOUND>::getBoundNeighbor(UInt i, UInt j) const {
    return neighbors_[i][j];
}

template<class ELEM, class BOUND>
typename GraphElement<ELEM,BOUND>::GraphElem*
    GraphElement<ELEM,BOUND>::getBoundNeighbor(UInt i, UInt j) {
    return neighbors_[i][j];
}

template<class ELEM, class BOUND>
void GraphElement<ELEM,BOUND>::constructNeighbors() {
    neighbors_.clear();
    neighbors_.resize(bounds_.size());
    for (UInt i = 0; i < bounds_.size(); i++) {
        for (UInt j = 0; j < bounds_[i]->numBounds(); j++) {
            if (bounds_[i]->getBound(j)->elem()->getId() != elem_->getId()) {
                neighbors_[i].push_back(bounds_[i]->getBound(j));
            }
        }
    }
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
        cout << "Neighbors:" << endl;
        for (UInt i = 0; i < neighbors_.size(); ++i) {
            cout << bounds_[i]->elem()->getId() << ":";
            for (UInt j = 0; j < neighbors_[i].size(); ++j) {
                cout << " " << neighbors_[i][j]->elem()->getId();
            }
            cout << endl;
        }
    }
}
