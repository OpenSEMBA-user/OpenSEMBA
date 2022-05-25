

#pragma once

#include "Surface.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class QuadrilateralBase : public virtual SurfaceBase {
public:
    virtual ~QuadrilateralBase() = default;
    std::size_t numberOfFaces   () const { return 4; }
    std::size_t numberOfVertices() const { return 4; }

    std::size_t numberOfSideVertices(const std::size_t f = 0) const { 
        return 2; 
    }
};

template<class T>
class Quadrilateral : public virtual Surface<T>,
                      public virtual QuadrilateralBase {
public:
    virtual ~Quadrilateral() = default;
};

} /* namespace Element */

typedef Element::QuadrilateralBase         Qua;
typedef Element::Quadrilateral<Math::Real> QuaR;
typedef Element::Quadrilateral<Math::Int > QuaI;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Quadrilateral.hpp"

