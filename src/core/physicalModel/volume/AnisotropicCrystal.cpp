

#include "physicalModel/volume/AnisotropicCrystal.h"

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

AnisotropicCrystal::AnisotropicCrystal(
        const Id matId,
        const std::string& name,
        const Math::Axis::Local& local,
        const Math::CVecR3& principalAxesRelativePermittivity,
        const Math::Real relativePermeability)
:   Identifiable<Id>(matId),
    PhysicalModel(name),
    Anisotropic(local) {
    principalAxesRelativePermittivity_ = principalAxesRelativePermittivity;
    relativePermeability_ = relativePermeability;
}

AnisotropicCrystal::AnisotropicCrystal(
    const AnisotropicCrystal& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs),
    Anisotropic(rhs) {
    principalAxesRelativePermittivity_ =
        rhs.principalAxesRelativePermittivity_;
    relativePermeability_ = rhs.relativePermeability_;
}

AnisotropicCrystal::~AnisotropicCrystal() {

}

const Math::CVecR3
    AnisotropicCrystal::getPrincipalAxesRelativePermittivity() const {
    return principalAxesRelativePermittivity_;
}

Math::Real AnisotropicCrystal::getRelativePermeability() const {
    return relativePermeability_;
}


Math::MatR33 AnisotropicCrystal::getRelPermittivityMatR() const {
    Math::MatR33 local;
    local.setInDiagonal(principalAxesRelativePermittivity_);
    return getLocalAxe().convertToGlobal(local);
}

Math::MatR33 AnisotropicCrystal::getRelPermeabilityMatR() const {
    Math::MatR33 local;
    local.setInDiagonal(Math::CVecR3(relativePermeability_));
    return getLocalAxe().convertToGlobal(local);
}

Math::MatR33 AnisotropicCrystal::getElectricConductivityMat() const {
    return Math::MatR33();
}

Math::MatR33 AnisotropicCrystal::getMagneticConductivityMat() const {
    return Math::MatR33();
}

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
