#pragma once

#include "Graph.h"
#include "geometry/coordinate/Coordinate.h"


namespace SEMBA {
namespace Geometry {
namespace Graph {

template<class ELEM, class BOUND>
class Vertices : public Graph<ELEM, BOUND> {
public:
    typedef ELEM  Elem;
    typedef BOUND Bound;
    typedef Element<Elem,Bound> GraphElem;
    typedef Element<Bound,Elem> GraphBound;

    Vertices(const std::vector<const Elem*>& elems);
    Vertices(const Vertices&) = default;
    virtual ~Vertices() = default;

    Vertices& operator=(const Vertices&);

    void splitBound(std::size_t i);
};

} /* namespace Graph */
} /* namespace Geometry */
} /* namespace SEMBA */

#include "Vertices.hpp"

