

#pragma once

#include "Coordinate.h"

namespace SEMBA {
namespace Geometry {
namespace Coordinate {

class Conformal : public virtual Coordinate<Math::Int,3> {
public:
    Conformal();
    Conformal(const Id id_,
              const Math::CVecI3& pos,
              const Math::Constants::CartesianAxis dir,
              const Math::Real length);
    Conformal(const Math::Constants::CartesianAxis dir,
              const Math::Real length);
    Conformal(const Conformal& rhs);
    virtual ~Conformal();

    SEMBA_CLASS_DEFINE_CLONE(Conformal);

    Conformal& operator=(const Conformal& rhs);

    bool operator==(const Base& rhs) const;

    Math::Constants::CartesianAxis getDir   () const { return dir_;    }
    Math::Real                     getLength() const { return length_; }

    CoordR3* toUnstructured(const Grid3&) const;

    void printInfo() const;

private:
    Math::Constants::CartesianAxis dir_;
    Math::Real                     length_;
};

} /* namespace Coordinate */

typedef Coordinate::Conformal CoordConf;

} /* namespace Geometry */
} /* namespace SEMBA */

