

#pragma once

#include <exception>

#include "Source.h"

namespace SEMBA {
namespace Source {

class PlaneWave : public Source<Geometry::Vol> {
public:
    PlaneWave();
    PlaneWave(Magnitude::Magnitude* magnitude,
              Geometry::Element::Group<Geometry::Vol> elem,
              Math::CVecR3 directionVector,
              Math::CVecR3 polarizationVector);
    PlaneWave(Magnitude::Magnitude* magnitude,
              Geometry::Element::Group<Geometry::Vol> elem,
              std::pair<Math::Real, Math::Real> directionAngles,
              std::pair<Math::Real, Math::Real> polarizationAngles);
    PlaneWave(Magnitude::Magnitude* magnitude,
              Geometry::Element::Group<Geometry::Vol> elem,
              Math::Int numberOfRandomPlanewaves,
              Math::Real relativeVariationOfRandomDelay);
    PlaneWave(const PlaneWave& rhs);
    virtual ~PlaneWave();

    SEMBA_CLASS_DEFINE_CLONE(PlaneWave);

    const std::string& getName() const;
    const Math::CVecR3& getPolarization() const;
    const Math::CVecR3& getDirection() const;
    Math::Real getTheta() const;
    Math::Real getPhi() const;
    Math::Real getAlpha() const;
    Math::Real getBeta() const;
    bool isRandomic() const;
    Math::Int getNumberOfRandomPlanewaves() const;
    Math::Real getRelativeVariationOfRandomDelay() const;

    Math::CVecR3 getElectricField(const Math::Real time) const;
    std::pair<Math::CVecR3,Math::CVecR3> getElectromagneticField(
            const Math::Real time) const;

private:
    Math::CVecR3 direction_;
    Math::CVecR3 polarization_;

    bool randomic_;
    Math::Int numberOfRandomPlanewaves_;
    Math::Real relativeVariationOfRandomDelay_;

    void init_(Math::CVecR3 direction, Math::CVecR3 polarization);
    static std::pair<Math::Real,Math::Real> cartesianToPolar(
            const Math::CVecR3& vec);
    static Math::CVecR3 polarToCartesian(Math::Real theta, Math::Real phi);
    static Math::Real reduceRadians(const Math::Real radianIn);
};

namespace Error {
namespace PlaneWave {

class Error : public std::exception {
public:
    Error() {}
    virtual ~Error() throw() {}
};

class ZeroPolarization : public Error {
public:
    ZeroPolarization() {}
    virtual ~ZeroPolarization() throw() {}

    const char* what() const throw() {
        return "PlaneWave: Polarization can't be zero.";
    }
};

class ZeroMagnitude : public Error {
public:
    ZeroMagnitude() {}
    virtual ~ZeroMagnitude() throw() {}

    const char* what() const throw() {
        return "PlaneWave: W. direction can't be zero.";
    }
};

class NotPerpendicular : public Error {
public:
    NotPerpendicular() {}
    virtual ~NotPerpendicular() throw() {}

    const char* what() const throw() {
        return ("PlaneWave: W. direction is not "
                "perpendicular to polarization.");
    }
};

} /* namespace PlaneWave */
} /* namespace Error */
} /* namespace Source */
} /* namespace SEMBA */

