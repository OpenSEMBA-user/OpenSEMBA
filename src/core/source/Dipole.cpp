

#include "Dipole.h"

namespace SEMBA {
namespace Source {

Dipole::Dipole(const Magnitude::Magnitude* magnitude,
               const Geometry::Element::Group<Geometry::Vol>& elem,
               Math::Real length,
               Math::CVecR3 orientation,
               Math::CVecR3 position)
:   SEMBA::Source::Base(magnitude),
    Geometry::Element::Group<const Geometry::Vol>(elem) {

    length_ = length;
    orientation_ = orientation;
    position_ = position;
    gaussDelay_ = 0.0;
    spreadSqrt2_ = 0.0;
}

Dipole::Dipole(const Dipole& rhs)
:   SEMBA::Source::Base(rhs),
    Geometry::Element::Group<const Geometry::Vol>(rhs) {

    length_ = rhs.length_;
    orientation_ = rhs.orientation_;
    position_ = rhs.position_;
    gaussDelay_ = rhs.gaussDelay_;
    spreadSqrt2_ = rhs.spreadSqrt2_;
}

const std::string& Dipole::getName() const {
    const static std::string res = "Dipole";
    return res;
}

} /* namespace Source */
} /* namespace SEMBA */
