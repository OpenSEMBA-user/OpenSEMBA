

#pragma once

#include <physicalModel/volume/Anisotropic.h>

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

// Described using Polder tensor model:
// https://en.wikipedia.org/wiki/Polder_tensor
class AnisotropicFerrite: public Anisotropic {
public:
    AnisotropicFerrite(const Id matId,
                             const std::string& name,
                             const Math::Axis::Local& local,
                             const Math::Real kappa,
                             const Math::Real relativePermeability,
                             const Math::Real relativePermittivity);
    AnisotropicFerrite(const AnisotropicFerrite&);
    virtual ~AnisotropicFerrite();

    SEMBA_CLASS_DEFINE_CLONE(AnisotropicFerrite);

    Math::MatR33 getRelPermittivityMatR() const;

    Math::MatR33 getRelPermeabilityMatR() const; // Math::Real part.
    Math::MatR33 getRelPermeabilityMatI() const; // Imaginary part.

    Math::MatR33 getElectricConductivityMat() const;
    Math::MatR33 getMagneticConductivityMat() const;
private:
    Math::Real kappa_;
    Math::Real relativePermeability_;
    Math::Real relativePermittivity_;
};

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */

