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

    SEMBA_CLASS_DEFINE_CLONE(AnisotropicCrystal);

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

