

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
//    gaussDelay_ = magnitude.getDelay();
//    spreadSqrt2_ = magnitude.getSpread() * sqrt(2.0);
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

Dipole::~Dipole() {

}

const std::string& Dipole::getName() const {
    const static std::string res = "Dipole";
    return res;
}

void Dipole::printInfo() const {
    std::cout << " ---- Dipole information ---- " << std::endl;
    SEMBA::Source::Base::printInfo();
    std::cout << " - Length: " << length_ << std::endl;
    std::cout << " - Orientation vector:";;
    orientation_.printInfo();
    std::cout << std::endl;
    std::cout << " - Position vector:";
    position_.printInfo();
    std::cout << std::endl;
}

} /* namespace Source */
} /* namespace SEMBA */
