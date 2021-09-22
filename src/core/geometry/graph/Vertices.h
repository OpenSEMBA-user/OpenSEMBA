

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

    Vertices();
    Vertices(const Vertices&);
    Vertices(const Group::Group<const Elem>& elems);
    virtual ~Vertices();

    Vertices<Elem,Bound>& init(const Group::Group<const Elem>& elems);

    Vertices& operator=(const Vertices&);

    void splitBound(std::size_t i);
};

} /* namespace Graph */
} /* namespace Geometry */
} /* namespace SEMBA */

#include "Vertices.hpp"

