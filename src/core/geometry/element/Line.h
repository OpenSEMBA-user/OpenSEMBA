

#pragma once

#include "math/simplex/Line.h"

#include "Element.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class LineBase : public virtual Base {
public:
    LineBase() {}
    virtual ~LineBase() {}

    inline std::size_t numberOfFaces   () const { return 2; }
    inline std::size_t numberOfVertices() const { return 2; }

    inline std::size_t numberOfSideVertices   (const std::size_t f = 0) const { 
        return 1; 
    }
    inline std::size_t numberOfSideCoordinates(const std::size_t f = 0) const { 
        return 1; 
    }

};

template<class T>
class Line : public virtual Element<T>,
             public virtual LineBase {
public:
    Line();
    virtual ~Line();
};

} /* namespace Element */

typedef Element::LineBase         Lin;
typedef Element::Line<Math::Real> LinR;
typedef Element::Line<Math::Int > LinI;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Line.hpp"

