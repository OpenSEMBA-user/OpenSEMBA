

#include "PlaneWave.h"

namespace SEMBA {
namespace Source {

PlaneWave::PlaneWave() {
    randomic_ = false;
    numberOfRandomPlanewaves_ = 0;
    relativeVariationOfRandomDelay_ = 0.0;
}

PlaneWave::PlaneWave(Magnitude::Magnitude* magnitude,
                     Geometry::Element::Group<Geometry::Vol> elem,
                     Math::CVecR3 direction,
                     Math::CVecR3 polarization)
:   SEMBA::Source::Base(magnitude),
    Geometry::Element::Group<const Geometry::Vol>(elem) {

    init_(direction, polarization);
}

PlaneWave::PlaneWave(
        Magnitude::Magnitude* magnitude,
        Geometry::Element::Group<Geometry::Vol> elem,
        std::pair<Math::Real, Math::Real> directionAngles,
        std::pair<Math::Real, Math::Real> polarizationAngles)
:   SEMBA::Source::Base(magnitude),
    Geometry::Element::Group<const Geometry::Vol>(elem) {

    Math::Real theta = directionAngles.first;
    Math::Real phi   = directionAngles.second;
    Math::Real alpha_ = polarizationAngles.first;
    Math::Real beta_  = polarizationAngles.second;

    Math::CVecR3 dirVec = polarToCartesian(theta, phi);
    Math::CVecR3 polVec = polarToCartesian(alpha_, beta_);
    init_(dirVec, polVec);
}

PlaneWave::PlaneWave(
        Magnitude::Magnitude* magnitude,
        Geometry::Element::Group<Geometry::Vol> elem,
        Math::Int numberOfRandomPlanewaves,
        Math::Real relativeVariationOfRandomDelay)
:   SEMBA::Source::Base(magnitude),
    Geometry::Element::Group<const Geometry::Vol>(elem) {

    randomic_ = true;
    numberOfRandomPlanewaves_ = numberOfRandomPlanewaves;
    relativeVariationOfRandomDelay_ = relativeVariationOfRandomDelay;
}


PlaneWave::PlaneWave(const PlaneWave& rhs)
:   SEMBA::Source::Base(rhs),
    Geometry::Element::Group<const Geometry::Vol>(rhs) {

    direction_ = rhs.direction_;
    polarization_ = rhs.polarization_;
    randomic_ = rhs.randomic_;
    numberOfRandomPlanewaves_ = rhs.numberOfRandomPlanewaves_;
    relativeVariationOfRandomDelay_ = rhs.relativeVariationOfRandomDelay_;
}

PlaneWave::~PlaneWave() {

}

const std::string& PlaneWave::getName() const {
    const static std::string res = "PlaneWave";
    return res;
}

const Math::CVecR3& PlaneWave::getPolarization() const {
    return polarization_;
}

const Math::CVecR3& PlaneWave::getDirection() const {
    return direction_;
}

Math::Real PlaneWave::getTheta() const {
    return cartesianToPolar (direction_).first;
}

Math::Real PlaneWave::getPhi() const {
    return cartesianToPolar (direction_).second;
}

Math::Real PlaneWave::getAlpha() const {
    return cartesianToPolar (polarization_).first;
}

Math::Real PlaneWave::getBeta() const {
    return cartesianToPolar (polarization_).second;
}

bool PlaneWave::isRandomic() const {
    return randomic_;
}

Math::Int PlaneWave::getNumberOfRandomPlanewaves() const {
    return numberOfRandomPlanewaves_;
}

Math::Real PlaneWave::getRelativeVariationOfRandomDelay() const {
    return relativeVariationOfRandomDelay_;
}

Math::CVecR3 PlaneWave::getElectricField(const Math::Real time) const {
    Math::CVecR3 res = polarization_ * getMagnitude()->evaluate(time);
    return res;
}

std::pair<Math::CVecR3, Math::CVecR3>
PlaneWave::getElectromagneticField(const Math::Real time) const {
    Math::CVecR3 electric = getElectricField(time);
    Math::CVecR3 magnetic = (direction_ ^ electric) *
                            Math::Constants::VACUUM_ADMITANCE;
    return std::pair<Math::CVecR3,Math::CVecR3>(electric, magnetic);
}

std::pair<Math::Real,Math::Real> PlaneWave::cartesianToPolar(
        const Math::CVecR3& v) {
    if (v.norm() == 0.0) {
        return std::pair<Math::Real,Math::Real>(0.0, 0.0);
    }
    Math::Real theta, phi;
    theta = std::acos(v(Math::Constants::z) / v.norm());
    if (v(Math::Constants::x) == 0.0) {
        if (v(Math::Constants::y) == 0.0) {
            phi = 0.0;
        } else if (v(Math::Constants::y) > 0.0) {
            phi = Math::Constants::pi_2;
        } else {
            phi = 3.0 * Math::Constants::pi_2;
        }
    } else {
        phi = std::atan2(v(Math::Constants::y), v(Math::Constants::x));
    }
    return std::pair<Math::Real,Math::Real>(theta,phi);
}

void PlaneWave::init_(Math::CVecR3 direction, Math::CVecR3 polarization) {
    direction_ = direction;
    polarization_ = polarization;

    randomic_ = false;
    numberOfRandomPlanewaves_ = 0;
    relativeVariationOfRandomDelay_ = 0.0;

    if (polarization_.norm() == 0) {
        throw Error::PlaneWave::ZeroPolarization();
    }
    if (direction_.norm() == 0) {
        throw Error::PlaneWave::ZeroMagnitude();
    }
    Math::Real dotProd = direction.dot(polarization);
    if (Math::Util::notEqual(dotProd, 0.0)) {
        throw Error::PlaneWave::NotPerpendicular();
    }
}

Math::CVecR3 PlaneWave::polarToCartesian(Math::Real theta, Math::Real phi) {
    return Math::CVecR3(
            std::sin(theta)*std::cos(phi),
            std::sin(theta)*std::sin(phi),
            std::cos(theta));
}

Math::Real PlaneWave::reduceRadians(const Math::Real radianIn) {
    Math::Real nVueltas, nVueltasComp, radianOut, Val2Pi;
    Val2Pi = (Math::Real) 2.0 * (Math::Real) acos((Math::Real) 0.0);
    nVueltas = radianIn/(Val2Pi);
    nVueltasComp = (Math::Real) floor(nVueltas);
    radianOut = radianIn - nVueltasComp*Val2Pi;
    return  radianOut;
}

} /* namespace Source */
} /* namespace SEMBA */
