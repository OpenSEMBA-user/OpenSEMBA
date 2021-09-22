

#pragma once

#include "Element.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class SurfaceBase : public virtual Base {
public:
    SurfaceBase() {};
    virtual ~SurfaceBase() {};
};

template<class T>
class Surface : public virtual Element<T>,
                public virtual SurfaceBase {
public:
    Surface();
    virtual ~Surface();

    bool isRectangular() const;
    bool isContainedInPlane() const;
    bool isContainedInPlane(const Math::Constants::CartesianPlane plane) const;

    virtual Math::Vector::Cartesian<T,3> getNormal() const;

    virtual void printInfo() const;
};

} /* namespace Element */

typedef Element::SurfaceBase         Surf;
typedef Element::Surface<Math::Real> SurfR;
typedef Element::Surface<Math::Int > SurfI;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Surface.hpp"

