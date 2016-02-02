// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

#include "VolumeClassic.h"

#include <cmath>

namespace SEMBA {
namespace PhysicalModel {

VolumeClassic::VolumeClassic(const Id matId,
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
}

VolumeClassic::VolumeClassic(const VolumeClassic& rhs)
:   Identifiable<Id>(rhs),
    PhysicalModel(rhs) {
    rEps_ = rhs.rEps_;
    rMu_ = rhs.rMu_;
    electricConductivity_ = rhs.electricConductivity_;
    magneticConudctivity_ = rhs.magneticConudctivity_;
}

VolumeClassic::~VolumeClassic() {

}

Math::Real VolumeClassic::getImpedance() const {
    if (rEps_ <= 0.0) {
        return std::numeric_limits<Math::Real>::infinity();
    }
    return std::sqrt((rMu_ * Math::Constants::mu0) /
                     (rEps_ * Math::Constants::eps0));
}

Math::Real VolumeClassic::getAdmitance() const {
    if (rMu_ <= 0.0) {
        return std::numeric_limits<Math::Real>::infinity();
    }
    return (1.0 / getImpedance());
}

Math::Real VolumeClassic::getRelativePermittivity() const {
    return rEps_;
}

Math::Real VolumeClassic::getRelativePermeability() const {
    return rMu_;
}

Math::Real VolumeClassic::getPermittivity() const {
    return (rEps_ * Math::Constants::eps0);
}

Math::Real VolumeClassic::getPermeability() const {
    return (rMu_ * Math::Constants::mu0);
}

Math::Real VolumeClassic::getElectricConductivity() const {
    return electricConductivity_;
}

Math::Real VolumeClassic::getMagneticConductivity() const {
    return magneticConudctivity_;
}

bool VolumeClassic::isVacuum() const {
    return (rEps_ == 1.0
            && rMu_ == 1.0
            && electricConductivity_ == 0.0
            && magneticConudctivity_ == 0.0);
}

void VolumeClassic::printInfo() const {
    std::cout << "--- VolumeClassic info ---" << std::endl;
    Volume::printInfo();
    std::cout << "Rel. elec. permittivity: "
              << getRelativePermittivity() << std::endl;
    std::cout << "Rel. magn. permeability: "
              << getRelativePermeability() << std::endl;
    std::cout << "Electric conductivity: "
              << getElectricConductivity() << std::endl;
    std::cout << "Magnetic conductivity: "
              << getMagneticConductivity() << std::endl;
}

} /* namespace PhysicalModel */
} /* namespace SEMBA */
