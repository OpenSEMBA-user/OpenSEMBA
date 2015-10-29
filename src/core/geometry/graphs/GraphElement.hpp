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
 * GraphElement.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: damarro
 */

#include "../graphs/GraphElement.h"

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
