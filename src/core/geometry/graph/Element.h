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

#pragma once

#include <vector>

namespace SEMBA {
namespace Geometry {
namespace Graph {

template<class ELEM, class BOUND>
class Element {
public:
    typedef ELEM  Elem;
    typedef BOUND Bound;
    typedef Element<Elem,Bound> GraphElem;
    typedef Element<Bound,Elem> GraphBound;

    Element(const Elem* elem, const std::size_t& numBounds = 0);
    Element(const Element&);

    Element* clone() const;

    bool visited() const { return vis_;  }
    void markVisited  () { vis_ =  true; }
    void unmarkVisited() { vis_ = false; }

    const Elem* elem() const { return elem_; }

    std::size_t numBounds() const { return bounds_.size(); }
    std::vector<const GraphBound*> getBounds() const;
    std::vector<GraphBound*>       getBounds()      { return bounds_;    }
    const GraphBound* getBound(std::size_t i) const { return bounds_[i]; }
    GraphBound*       getBound(std::size_t i)       { return bounds_[i]; }
    void setBounds(GraphBound* bound);
    void setBounds(std::vector<GraphBound*> bounds)  { bounds_ = bounds;   }
    void setBound (std::size_t i, GraphBound* bound) { bounds_[i] = bound; }
    void addBound (GraphBound* bound)                {
        bounds_.push_back(bound);
    }

    std::size_t numNeighbors() const { return neighbors_.size(); }
    const GraphElem* getNeighbor(std::size_t i) const { return neighbors_[i]; }
    GraphElem*       getNeighbor(std::size_t i)       { return neighbors_[i]; }

    std::size_t numBoundNeighbors(std::size_t i) const {
        return boundNeighbors_[i].size();
    }
    const GraphElem* getBoundNeighbor(std::size_t i, std::size_t j) const;
    GraphElem*       getBoundNeighbor(std::size_t i, std::size_t j);

    void constructNeighbors();

    void printInfo() const;

private:
    bool vis_;

    const Elem* elem_;
    std::vector<GraphBound*>             bounds_;
    std::vector<GraphElem*>              neighbors_;
    std::vector<std::vector<GraphElem*>> boundNeighbors_;
};

} /* namespace Graph */
} /* namespace Geometry */
} /* namespace SEMBA */

#include "Element.hpp"

