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
 * PMVolumeAnisotropic.h
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_

#include "PMVolume.h"
#include "math/LocalAxes.h"

class PMVolumeAnisotropic: public PMVolume {
public:
    enum class Model {
        crystal,
        ferrite
    };

    PMVolumeAnisotropic(
            const MatId matId,
            const string& name,
            const LocalAxes& local);
    virtual ~PMVolumeAnisotropic();

    LocalAxes getLocalAxe() const;
    virtual MatR33 getRelPermittivityMatR() const = 0;
    virtual MatR33 getRelPermeabilityMatR() const = 0;
    virtual MatR33 getElectricConductivityMat() const = 0;
    virtual MatR33 getMagneticConductivityMat() const = 0;

private:
    LocalAxes localAxe_;
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_ */
