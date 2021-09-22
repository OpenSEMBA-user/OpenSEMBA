

#pragma once

#include "Coordinate.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

class Relative : public virtual Coordinate<Math::Int,3> {
public:
    Relative();
    Relative(const Id,
             const Math::CVecR3&);
    Relative(const Id,
             const Math::CVecI3&,
             const Math::CVecR3&);
    Relative(const Math::CVecR3&);
    Relative(const Relative&);
    virtual ~Relative();

    SEMBA_CLASS_DEFINE_CLONE(Relative);

    Relative& operator=(const Relative& rhs);

    bool operator==(const Base& rhs) const;

    Math::CVecR3&       rel()       { return rel_; }
    const Math::CVecR3& rel() const { return rel_; }

    CoordR3* toUnstructured(const Grid3&) const;

    void printInfo() const;

private:
    Math::CVecR3 rel_;
};

} /* namespace Coordinate */

typedef Coordinate::Relative CoordRel;

} /* namespace Geometry */
} /* namespace SEMBA */

