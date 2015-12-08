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

#include "Vertices.h"

namespace SEMBA {
namespace Geometry {
namespace Graph {

template<class ELEM, class BOUND>
Vertices<ELEM,BOUND>::Vertices() {

}

template<class ELEM, class BOUND>
Vertices<ELEM,BOUND>::Vertices(const Vertices& rhs)
:   Graph<ELEM,BOUND>(rhs) {

}

template<class ELEM, class BOUND>
Vertices<ELEM,BOUND>::~Vertices() {

}

template<class ELEM, class BOUND>
Vertices<ELEM,BOUND>& Vertices<ELEM,BOUND>::init(
        const Group::Group<typename const Vertices<ELEM,BOUND>::Elem>& elems) {
    const Bound* coord;
    GraphElem*   elemPtr;
    GraphBound*  boundPtr;
    std::map<CoordId, GraphBound*> map;
    this->elems_.clear();
    this->bounds_.clear();
    for (std::size_t s = 0; s < elems.size(); s++) {
        elemPtr = new GraphElem(elems(s), elems(s)->numberOfCoordinates());
        this->elems_.push_back(elemPtr);
        for (std::size_t v = 0; v < elems(s)->numberOfCoordinates(); v++) {
            coord = elems(s)->getV(v);
            if (map.count(coord->getId()) == 0) {
                boundPtr = new GraphBound(coord);
                this->bounds_.push_back(boundPtr);
                map[coord->getId()] = boundPtr;
            }
            boundPtr = map[coord->getId()];
            elemPtr->setBound(v, boundPtr);
            boundPtr->addBound(elemPtr);
        }
    }
    map.clear();
    for (std::size_t i = 0; i < this->elems_.size(); i++) {
        this->elems_[i]->constructNeighbors();
    }
    for (std::size_t i = 0; i < this->bounds_.size(); i++) {
        this->bounds_[i]->constructNeighbors();
    }
    return *this;
}

template<class ELEM, class BOUND>
Vertices<ELEM,BOUND>& Vertices<ELEM,BOUND>::operator=(
        const Vertices& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Graph<ELEM,BOUND>::operator=(rhs);
    return *this;
}

template<class ELEM, class BOUND>
void Vertices<ELEM,BOUND>::splitBound(std::size_t i) {
    std::vector<GraphElem*> oldAdj = this->bounds_[i]->getBounds();
    if (oldAdj.empty()) {
        return;
    }
    this->bounds_[i]->setBounds(oldAdj[0]);
    this->bounds_[i]->getBound(0)->constructNeighbors();
    this->bounds_[i]->constructNeighbors();
    for (std::size_t j = 1; j < oldAdj.size(); j++) {
        this->bounds_.push_back(new GraphBound(*this->bounds_[i]));
        this->bounds_.back()->setBounds(oldAdj[j]);
        if (oldAdj[j]->getBound(0)->elem()->getId() ==
            this->bounds_[i]->elem()->getId()) {
            oldAdj[j]->setBound(0, this->bounds_.back());
        }
        if (oldAdj[j]->getBound(1)->elem()->getId() ==
            this->bounds_[i]->elem()->getId()) {
            oldAdj[j]->setBound(1, this->bounds_.back());
        }
        this->bounds_.back()->getBound(0)->constructNeighbors();
        this->bounds_.back()->constructNeighbors();
    }
}

} /* namespace Graph */
} /* namespace Geometry */
} /* namespace SEMBA */
