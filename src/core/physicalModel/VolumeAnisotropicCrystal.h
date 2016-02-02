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

#ifndef SEMBA_PHYSICALMODEL_VOLUMEANISOTROPICCRYSTAL_H_
#define SEMBA_PHYSICALMODEL_VOLUMEANISOTROPICCRYSTAL_H_

#include "VolumeAnisotropic.h"

namespace SEMBA {
namespace PhysicalModel {

// Described in: https://courses.cit.cornell.edu/ece303/Lectures/lecture17.pdf
class VolumeAnisotropicCrystal: public VolumeAnisotropic {
public:
    VolumeAnisotropicCrystal(
            const Id matId,
            const std::string& name,
            const Math::Axis::Local& local,
            const Math::CVecR3& principalAxesRelativePermittivity,
            const Math::Real relativePermeability);
    VolumeAnisotropicCrystal(const VolumeAnisotropicCrystal&);
    virtual ~VolumeAnisotropicCrystal();

    SEMBA_CLASS_DEFINE_CLONE(VolumeAnisotropicCrystal);

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

} /* namespace PhysicalModel */
} /* namespace SEMBA */

#endif /* SEMBA_PHYSICALMODEL_VOLUMEANISOTROPICCRYSTAL_H_ */
