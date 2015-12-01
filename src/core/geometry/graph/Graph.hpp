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

#include <geometry/graph/Graph.h>
#include <map>
#include <queue>

namespace SEMBA {
namespace Geometry {
namespace Graph {

template<class ELEM, class BOUND>
Graph<ELEM,BOUND>::Graph() {

}

template<class ELEM, class BOUND>
Graph<ELEM,BOUND>::Graph(const Graph& rhs) {
    cloneInfo(rhs);
}

template<class ELEM, class BOUND>
Graph<ELEM,BOUND>::~Graph() {
    for (Size i = 0; i < elems_.size(); i++) {
        delete elems_[i];
    }
    for (Size i = 0; i < bounds_.size(); i++) {
        delete bounds_[i];
    }
}

template<class ELEM, class BOUND>
Graph<ELEM,BOUND>& Graph<ELEM,BOUND>::operator=(const Graph& rhs) {
    if (this == &rhs) {
        return *this;
    }
    cloneInfo(rhs);
    return *this;
}

template<class ELEM, class BOUND>
void Graph<ELEM,BOUND>::printInfo() const {
    std::cout << "--- Graph Info ---" << std::endl;
    std::cout << "Elems: " << elems_.size() << std::endl;
    for (Size i = 0; i < elems_.size(); i++) {
        elems_[i]->printInfo();
    }
    std::cout << "Bounds: " << bounds_.size() << std::endl;
    for (Size i = 0; i < bounds_.size(); i++) {
        bounds_[i]->printInfo();
    }
}

template<class ELEM, class BOUND>
void Graph<ELEM,BOUND>::resetVisited() {
    for (Size i = 0; i < elems_.size(); i++) {
        elems_[i]->unmarkVisited();
    }
    for (Size i = 0; i < bounds_.size(); i++) {
        bounds_[i]->unmarkVisited();
    }
}

template<class ELEM, class BOUND>
std::vector<std::vector<const ELEM*>>
        Graph<ELEM,BOUND>::getConnectedComponents() {
    resetVisited();
    std::vector<std::vector<const Elem*>> res;
    for (Size i = 0; i < elems_.size(); i++) {
        if (!elems_[i]->visited()) {
            std::vector<const Elem*> comp;
            std::queue<GraphElem*> q;
            q.push(elems_[i]);
            elems_[i]->markVisited();
            while (!q.empty()) {
                GraphElem* elem = q.front();
                q.pop();
                comp.push_back(elem->elem());
                elem->markVisited();
                for (Size n = 0; n < elem->numNeighbors(); n++) {
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
void Graph<ELEM,BOUND>::cloneInfo(const Graph& rhs) {
    std::map<GraphElem* , GraphElem* > mapElems;
    std::map<GraphBound*, GraphBound*> mapBounds;

    elems_.resize(rhs.elems_.size());
    for (Size i = 0; i < rhs.elems_.size(); i++) {
        elems_[i] = rhs.elems_[i]->clone();
        mapElems[rhs.elems_[i]] = elems_[i];
    }
    bounds_.resize(rhs.bounds_.size());
    for (Size i = 0; i < rhs.bounds_.size(); i++) {
        bounds_[i] = rhs.bounds_[i]->clone();
        mapBounds[rhs.bounds_[i]] = bounds_[i];
    }

    for (Size i = 0; i < elems_.size(); i++) {
        for (Size j = 0; j < elems_[i]->numBounds(); j++) {
            elems_[i]->setBound(j, mapBounds[elems_[i]->getBound(j)]);
        }
    }
    for (Size i = 0; i < bounds_.size(); i++) {
        for (Size j = 0; j < bounds_[i]->numBounds(); j++) {
            bounds_[i]->setBound(j, mapElems[bounds_[i]->getBound(j)]);
        }
    }

    for (Size i = 0; i < this->elems_.size(); i++) {
        this->elems_[i]->constructNeighbors();
    }
    for (Size i = 0; i < this->bounds_.size(); i++) {
        this->bounds_[i]->constructNeighbors();
    }
}

} /* namespace Graph */
} /* namespace Geometry */
} /* namespace SEMBA */
