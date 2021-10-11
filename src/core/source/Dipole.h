

#pragma once

#include "Source.h"

namespace SEMBA {
namespace Source {

class Dipole : public Source<Geometry::Vol> {
public:
    Dipole(const Magnitude::Magnitude* magnitude,
           const Geometry::Element::Group<Geometry::Vol>& elem,
           Math::Real   length,
           Math::CVecR3 orientation,
           Math::CVecR3 position);
    Dipole(const Dipole& rhs);
    
    SEMBA_CLASS_DEFINE_CLONE(Dipole);

    const std::string& getName() const;
protected:
    Math::Real length_;
    Math::CVecR3 orientation_;
    Math::CVecR3 position_;
    Math::Real gaussDelay_;
    Math::Real spreadSqrt2_;
};

} /* namespace Source */
} /* namespace SEMBA */

