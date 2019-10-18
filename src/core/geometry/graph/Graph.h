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

#ifndef SEMBA_GEOMETRY_GRAPH_GRAPH_H_
#define SEMBA_GEOMETRY_GRAPH_GRAPH_H_

#include <vector>

#include "group/Group.h"

#include "Element.h"

namespace SEMBA {
namespace Geometry {
namespace Graph {

template<class ELEM, class BOUND>
class Graph {
public:
    typedef ELEM  Elem;
    typedef BOUND Bound;
    typedef Element<Elem,Bound> GraphElem;
    typedef Element<Bound,Elem> GraphBound;

    Graph();
    Graph(const Graph&);
    virtual ~Graph();

    virtual Graph& init(const Group::Group<const Elem>& elems) = 0;

    //Graph& operator=(const Graph&);

    std::size_t numElems () const { return elems_.size();  }
    std::size_t numBounds() const { return bounds_.size(); }

    const GraphElem*  elem (std::size_t i) const { return elems_ [i]; }
    GraphElem*        elem (std::size_t i)       { return elems_ [i]; }
    const GraphBound* bound(std::size_t i) const { return bounds_[i]; }
    GraphBound*       bound(std::size_t i)       { return bounds_[i]; }

    void resetVisited();
    std::vector<std::vector<const Elem*>> getConnectedComponents();

    void printInfo() const;

protected:
    std::vector<GraphElem* > elems_;
    std::vector<GraphBound*> bounds_;

private:
    void cloneInfo(const Graph&);
};

} /* namespace Graph */
} /* namespace Geometry */
} /* namespace SEMBA */

#include "Graph.hpp"

#endif /* SEMBA_GEOMETRY_GRAPH_GRAPH_H_ */
