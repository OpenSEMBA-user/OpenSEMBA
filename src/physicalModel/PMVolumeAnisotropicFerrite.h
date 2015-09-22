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
 * PMVolumeAnistropicFerrite.h
 *
 *  Created on: Aug 28, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICFERRITE_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICFERRITE_H_

#include "PMVolumeAnisotropic.h"

// Described using Polder tensor model:
// https://en.wikipedia.org/wiki/Polder_tensor
class PMVolumeAnisotropicFerrite: public PMVolumeAnisotropic {
public:
    PMVolumeAnisotropicFerrite(
            const MatId matId,
            const string& name,
            const LocalAxes& local,
            const Real kappa,
            const Real relativePermeability,
            const Real relativePermittivity);
    virtual ~PMVolumeAnisotropicFerrite();

    DEFINE_CLONE(PMVolumeAnisotropicFerrite);

    MatR33 getRelPermittivityMatR() const;

    MatR33 getRelPermeabilityMatR() const; // Real part.
    MatR33 getRelPermeabilityMatI() const; // Imaginary part.

    MatR33 getElectricConductivityMat() const;
    MatR33 getMagneticConductivityMat() const;
private:
    Real kappa_;
    Real relativePermeability_;
    Real relativePermittivity_;
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICFERRITE_H_ */
