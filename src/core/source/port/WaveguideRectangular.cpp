

#include "WaveguideRectangular.h"
#include "geometry/Bound.h"

namespace SEMBA {
namespace Source {
namespace Port {

WaveguideRectangular::WaveguideRectangular(
        const std::unique_ptr<Magnitude::Magnitude>& magn,
        const Target& elem,
        const ExcitationMode excMode,
        const std::pair<size_t,size_t> mode)
: Waveguide(magn, elem, excMode, mode) {

    box_ = Geometry::getBound(elem.begin(), elem.end());

    if (mode.first == 0 && mode.second == 0) {
        throw std::logic_error("At least one mode must be non-zero.");
    }
}

WaveguideRectangular::WaveguideRectangular(const WaveguideRectangular& rhs)
:   // SEMBA::Source::Source(rhs),
    //Geometry::Element::Group<const Geometry::Surf>(rhs),
    Waveguide(rhs) {
    box_ = rhs.box_;
}

WaveguideRectangular::~WaveguideRectangular() {

}

std::string WaveguideRectangular::getName() const {
    return "Rectangular_waveguide_port";
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
    const Target& constGroupElems) {
    Waveguide::setTarget(constGroupElems);
    box_ = Geometry::getBound(constGroupElems.begin(), constGroupElems.end());
}

Math::CVecR3 WaveguideRectangular::getOrigin() const {
    return box_.getMin();
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */
