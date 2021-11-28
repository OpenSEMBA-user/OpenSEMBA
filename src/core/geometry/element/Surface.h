

#pragma once

#include "Element.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class SurfaceBase : public virtual Base {
public:
    SurfaceBase() = default;
    virtual ~SurfaceBase() = default;

    virtual Math::CVecR3 getNormal() const = 0;
};

template<class T>
class Surface : public virtual Element<T>,
                public virtual SurfaceBase {
public:
    Surface() = default;
    virtual ~Surface() = default;

    bool isRectangular() const;
    bool isContainedInPlane() const;
    bool isContainedInPlane(const Math::Constants::CartesianPlane plane) const;

    virtual Math::CVecR3 getNormal() const override; 
};

} /* namespace Element */

typedef Element::SurfaceBase         Surf;
typedef Element::Surface<Math::Real> SurfR;
typedef Element::Surface<Math::Int > SurfI;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Surface.hpp"

