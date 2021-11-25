

#pragma once

#include <physicalModel/volume/Anisotropic.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

// Described in: https://courses.cit.cornell.edu/ece303/Lectures/lecture17.pdf
class AnisotropicCrystal: public Anisotropic {
public:
    AnisotropicCrystal(
            const Id matId,
            const std::string& name,
            const Math::Axis::Local& local,
            const Math::CVecR3& principalAxesRelativePermittivity,
            const Math::Real relativePermeability);
    AnisotropicCrystal(const AnisotropicCrystal&);
    virtual ~AnisotropicCrystal();

    virtual std::unique_ptr<PhysicalModel> clone() const override {
        return std::make_unique<AnisotropicCrystal>(*this);
    }

    const Math::CVecR3 getPrincipalAxesRelativePermittivity() const;
    Math::Real getRelativePermeability() const;

    Math::MatR33 getRelPermittivityMatR() const;
    Math::MatR33 getRelPermeabilityMatR() const;
    Math::MatR33 getElectricConductivityMat() const;
    Math::MatR33 getMagneticConductivityMat() const;
private:
    Math::CVecR3 principalAxesRelativePermittivity_;
    Math::Real relativePermeability_;
};

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

