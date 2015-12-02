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
 * GraphBase.hpp
 *
 *  Created on: 17/5/2015
 *      Author: Daniel
 */

#include "../graphs/GraphBase.h"

template<class ELEM, class BOUND>
GraphBase<ELEM,BOUND>::GraphBase() {

}

template<class ELEM, class BOUND>
GraphBase<ELEM,BOUND>::GraphBase(const GraphBase& rhs) {
    cloneInfo(rhs);
}

template<class ELEM, class BOUND>
GraphBase<ELEM,BOUND>::~GraphBase() {
    for (UInt i = 0; i < elems_.size(); i++) {
        delete elems_[i];
    }
    for (UInt i = 0; i < bounds_.size(); i++) {
        delete bounds_[i];
    }
}

template<class ELEM, class BOUND>
GraphBase<ELEM,BOUND>& GraphBase<ELEM,BOUND>::operator=(const GraphBase& rhs) {
    if (this == &rhs) {
        return *this;
    }
    cloneInfo(rhs);
    return *this;
}

template<class ELEM, class BOUND>
void GraphBase<ELEM,BOUND>::printInfo() const {
    cout << "--- Graph Info ---" << endl;
    cout << "Elems: " << elems_.size() << endl;
    for (UInt i = 0; i < elems_.size(); i++) {
        elems_[i]->printInfo();
    }
    cout << "Bounds: " << bounds_.size() << endl;
    for (UInt i = 0; i < bounds_.size(); i++) {
        bounds_[i]->printInfo();
    }
}

template<class ELEM, class BOUND>
void GraphBase<ELEM,BOUND>::resetVisited() {
    for (UInt i = 0; i < elems_.size(); i++) {
        elems_[i]->unmarkVisited();
    }
    for (UInt i = 0; i < bounds_.size(); i++) {
        bounds_[i]->unmarkVisited();
    }
}

template<class ELEM, class BOUND>
vector<vector<const ELEM*>> GraphBase<ELEM,BOUND>::getConnectedComponents() {
    resetVisited();
    vector<vector<const Elem*>> res;
    for (UInt i = 0; i < elems_.size(); i++) {
        if (!elems_[i]->visited()) {
            vector<const Elem*> comp;
            queue<GraphElem*> q;
            q.push(elems_[i]);
            elems_[i]->markVisited();
            while (!q.empty()) {
                GraphElem* elem = q.front();
                q.pop();
                comp.push_back(elem->elem());
                elem->markVisited();
                for (UInt n = 0; n < elem->numNeighbors(); n++) {
                    if (!elem->getNeighbor(n)->visited()) {
                        q.push(elem->getNeighbor(n));
                        elem->getNeighbor(n)->markVisited();
                    }
                }
            }
            res.push_back(comp);
        }
    }
    resetVisited();
    return res;
}

template<class ELEM, class BOUND>
void GraphBase<ELEM,BOUND>::cloneInfo(const GraphBase& rhs) {
    map<GraphElem* , GraphElem* > mapElems;
    map<GraphBound*, GraphBound*> mapBounds;

    elems_.resize(rhs.elems_.size());
    for (UInt i = 0; i < rhs.elems_.size(); i++) {
        elems_[i] = rhs.elems_[i]->clone();
        mapElems[rhs.elems_[i]] = elems_[i];
    }
    bounds_.resize(rhs.bounds_.size());
    for (UInt i = 0; i < rhs.bounds_.size(); i++) {
        bounds_[i] = rhs.bounds_[i]->clone();
        mapBounds[rhs.bounds_[i]] = bounds_[i];
    }

    for (UInt i = 0; i < elems_.size(); i++) {
        for (UInt j = 0; j < elems_[i]->numBounds(); j++) {
            elems_[i]->setBound(j, mapBounds[elems_[i]->getBound(j)]);
        }
    }
    for (UInt i = 0; i < bounds_.size(); i++) {
        for (UInt j = 0; j < bounds_[i]->numBounds(); j++) {
            bounds_[i]->setBound(j, mapElems[bounds_[i]->getBound(j)]);
        }
    }

    for (UInt i = 0; i < this->elems_.size(); i++) {
        this->elems_[i]->constructNeighbors();
    }
    for (UInt i = 0; i < this->bounds_.size(); i++) {
        this->bounds_[i]->constructNeighbors();
    }
}
