#include <physicalModel/volume/Classic.h>
#include <cmath>

namespace SEMBA {
namespace PhysicalModel {
namespace Volume {

Classic::Classic(const Id matId,
                 const std::string& name,
                 const Math::Real relativePermittivity,
                 const Math::Real relativePermeability,
                 const Math::Real electricConductivity,
                 const Math::Real magneticConductivity)
:   Identifiable<Id>(matId), 
    PhysicalModel(name) {
    rEps_ = relativePermittivity;
    rMu_ = relativePermeability;
    electricConductivity_ = electricConductivity;
    magneticConudctivity_ = magneticConductivity;
    if (relativePermittivity< 1.0 ||
        relativePermeability < 1.0 ||
        electricConductivity < 0.0 ||
        magneticConductivity < 0.0
     ) {
        std::cerr << std::endl << "WARNING: "
            << "Material " << matId << ": " << name << " has wrong "
            << "permittivity, permeability, or conductivity." << std::endl;
    }
}

Classic::Classic(const Classic& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {
    rEps_ = rhs.rEps_;
    rMu_ = rhs.rMu_;
    electricConductivity_ = rhs.electricConductivity_;
    magneticConudctivity_ = rhs.magneticConudctivity_;
}

Classic::~Classic() {

}

Math::Real Classic::getImpedance() const {
    if (rEps_ <= 0.0) {
        return std::numeric_limits<Math::Real>::infinity();
    }
    return std::sqrt((rMu_ * Math::Constants::mu0) /
                     (rEps_ * Math::Constants::eps0));
}

Math::Real Classic::getAdmitance() const {
    if (rMu_ <= 0.0) {
        return std::numeric_limits<Math::Real>::infinity();
    }
    return (1.0 / getImpedance());
}

Math::Real Classic::getRelativePermittivity() const {
    return rEps_;
}

Math::Real Classic::getRelativePermeability() const {
    return rMu_;
}

Math::Real Classic::getPermittivity() const {
    return (rEps_ * Math::Constants::eps0);
}

Math::Real Classic::getPermeability() const {
    return (rMu_ * Math::Constants::mu0);
}

Math::Real Classic::getElectricConductivity() const {
    return electricConductivity_;
}

Math::Real Classic::getMagneticConductivity() const {
    return magneticConudctivity_;
}

bool Classic::isVacuum() const {
    return (rEps_ == 1.0
            && rMu_ == 1.0
            && electricConductivity_ == 0.0
            && magneticConudctivity_ == 0.0);
}

} /* namespace Volume */
} /* namespace PhysicalModel */
} /* namespace SEMBA */
