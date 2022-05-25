

#pragma once

#include <exception>

#include "Source.h"

namespace SEMBA {
namespace Source {

class PlaneWave : public Source {
public:
    PlaneWave() = default;
    PlaneWave(const std::unique_ptr<Magnitude::Magnitude>&,
              const Target& elem,
              const Math::CVecR3& directionVector,
              const Math::CVecR3& polarizationVector);
    PlaneWave(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
              const Target& elem,
              std::pair<Math::Real, Math::Real> directionAngles,
              std::pair<Math::Real, Math::Real> polarizationAngles);
    // TODO: Probably would have sense as another source type
    PlaneWave(const std::unique_ptr<Magnitude::Magnitude>& magnitude,
              const Target& elem,
              Math::Int numberOfRandomPlanewaves,
              Math::Real relativeVariationOfRandomDelay);
    
    virtual ~PlaneWave() = default;

    virtual std::unique_ptr<Source> clone() const override {
        return std::make_unique<PlaneWave>(*this);
    }

    std::string getName() const { return "PlaneWave"; };
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
    std::pair<Math::CVecR3,Math::CVecR3> getElectromagneticField(const Math::Real time) const;

private:
    Math::CVecR3 direction_;
    Math::CVecR3 polarization_;

    bool randomic_ = false;
    Math::Int numberOfRandomPlanewaves_ = 0;
    Math::Real relativeVariationOfRandomDelay_ = 0.0;

    void init_(Math::CVecR3 direction, Math::CVecR3 polarization);
    static std::pair<Math::Real,Math::Real> cartesianToPolar(const Math::CVecR3& vec);
    static Math::CVecR3 polarToCartesian(Math::Real theta, Math::Real phi);
    static Math::Real reduceRadians(const Math::Real radianIn);
};

// TODO: Borrar namespace de error y refactor usos
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

