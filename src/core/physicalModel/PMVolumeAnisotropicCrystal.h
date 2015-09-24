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
/*
 * PMVolumeAnisotropicCrystal.h
 *
 *  Created on: Aug 28, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICCRYSTAL_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICCRYSTAL_H_

#include "PMVolumeAnisotropic.h"

// Described in: https://courses.cit.cornell.edu/ece303/Lectures/lecture17.pdf
class PMVolumeAnisotropicCrystal: public PMVolumeAnisotropic {
public:
    PMVolumeAnisotropicCrystal(
            const MatId matId,
            const string& name,
            const LocalAxes& local,
            const CVecR3& principalAxesRelativePermittivity,
            const Real relativePermeability);
    virtual ~PMVolumeAnisotropicCrystal();

    DEFINE_CLONE(PMVolumeAnisotropicCrystal);

    const CVecR3 getPrincipalAxesRelativePermittivity() const;
    Real getRelativePermeability() const;

    MatR33 getRelPermittivityMatR() const;
    MatR33 getRelPermeabilityMatR() const;
    MatR33 getElectricConductivityMat() const;
    MatR33 getMagneticConductivityMat() const;
private:
    CVecR3 principalAxesRelativePermittivity_;
    Real relativePermeability_;
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICCRYSTAL_H_ */
