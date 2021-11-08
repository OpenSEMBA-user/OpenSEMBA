

#include "WaveguideRectangular.h"

namespace SEMBA {
namespace Source {
namespace Port {

WaveguideRectangular::WaveguideRectangular(
        Magnitude::Magnitude* magn,
        const Geometry::Element::Group<const Geometry::Surf>& elem,
        const ExcitationMode excMode,
        const std::pair<size_t,size_t> mode)
:   SEMBA::Source::Base(magn),
    Geometry::Element::Group<const Geometry::Surf>(elem),
    Waveguide(magn, elem, excMode, mode) {

    box_ = this->getBound();

    if (mode.first == 0 && mode.second == 0) {
        throw std::logic_error("At least one mode must be non-zero.");
    }
}

WaveguideRectangular::WaveguideRectangular(const WaveguideRectangular& rhs)
:   SEMBA::Source::Base(rhs),
    Geometry::Element::Group<const Geometry::Surf>(rhs),
    Waveguide(rhs) {
    box_ = rhs.box_;
}

WaveguideRectangular::~WaveguideRectangular() {

}

const std::string& WaveguideRectangular::getName() const {
    const static std::string res = "Rectangular_waveguide_port";
    return res;
}

Math::CVecR3 WaveguideRectangular::getWeight(
        const Math::CVecR3& pos) const {
    // Return normalized weights for electric field components.
    static const Math::Real pi = acos(-1.0);
    Math::CVecR3 res;
    Math::CVecR3 rPos = pos - getOrigin();
    const Math::Real m = pi * getMode().first / getWidth();
    const Math::Real n = pi * getMode().second / getHeight();
    Math::Real normFactor = m;
    if (n > m) {
        normFactor = n;
    }
    //const Math::Real betaC = sqrt(pow(m,2) + pow(n,2));
    if (getExcitationMode() == Waveguide::ExcitationMode::TE) {
        res(Math::Constants::x) =   n * cos(m * rPos(Math::Constants::x)) *
                                        sin(n * rPos(Math::Constants::y)) /
                                        normFactor;
        res(Math::Constants::y) =   m * sin(m * rPos(Math::Constants::x)) *
                                        cos(n * rPos(Math::Constants::y)) /
                                        normFactor;
        res(Math::Constants::z) = (Math::Real) 0.0;
    } else {
        res(Math::Constants::x) = - m * cos(m * rPos(Math::Constants::x)) *
                                        sin(n * rPos(Math::Constants::y)) /
                                        normFactor;
        res(Math::Constants::y) = - m * sin(m * rPos(Math::Constants::x)) *
                                        cos(n * rPos(Math::Constants::y)) /
                                        normFactor;
        res(Math::Constants::z) = (Math::Real) 0.0;
    }
    return res;
}

Math::Real WaveguideRectangular::getWidth() const {
    Math::CVecR3 origin = getOrigin();
    Math::CVecR3 max = box_.getMax();
    return max(Math::Constants::x) - origin(Math::Constants::x);
}

Math::Real WaveguideRectangular::getHeight() const {
    Math::CVecR3 origin = getOrigin();
    Math::CVecR3 max = box_.getMax();
    return max(Math::Constants::y) - origin(Math::Constants::y);
}

void WaveguideRectangular::set(
    const Geometry::Element::Group<const Geometry::Elem>& constGroupElems) {
    Waveguide::set(constGroupElems);
    box_ = this->getBound();
}

Math::CVecR3 WaveguideRectangular::getOrigin() const {
    return box_.getMin();
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */
