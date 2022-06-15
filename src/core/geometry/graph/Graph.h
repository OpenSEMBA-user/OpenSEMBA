#pragma once

#include <vector>


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

    Graph() = default;
    Graph(const Graph&);
    virtual ~Graph();

    std::size_t numElems () const { return elems_.size();  }
    std::size_t numBounds() const { return bounds_.size(); }

    const GraphElem*  elem (std::size_t i) const { return elems_ [i]; }
    GraphElem*        elem (std::size_t i)       { return elems_ [i]; }
    const GraphBound* bound(std::size_t i) const { return bounds_[i]; }
    GraphBound*       bound(std::size_t i)       { return bounds_[i]; }

    void resetVisited();
    std::vector<std::vector<const Elem*>> getConnectedComponents();

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

