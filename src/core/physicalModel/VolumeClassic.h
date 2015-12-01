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
 * PMVolumeClassic.h
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMECLASSIC_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMECLASSIC_H_

#include "PMVolume.h"

class PMVolumeClassic : public PMVolume {
public:
    PMVolumeClassic(
            const MatId matId,
            const string& name,
            const Real relativePermittivity = 1.0,
            const Real relativePermeability = 1.0,
            const Real electricConductivity = 0.0,
            const Real magneticConductivity = 0.0);
    virtual ~PMVolumeClassic();

    DEFINE_CLONE(PMVolumeClassic);

    Real getRelativePermittivity() const;
    Real getPermittivity() const;
    Real getRelativePermeability() const;
    Real getPermeability() const;
    Real getImpedance() const;
    Real getAdmitance() const;
    Real getElectricConductivity() const;
    Real getMagneticConductivity() const;
    bool isVacuum() const;

    void printInfo() const;
private:
    Real rEps_; // Rel. permittivity @ infte. freq.
    Real rMu_; // Rel. permeability @ infte. freq.
    Real electricConductivity_;
    Real magneticConudctivity_;
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMECLASSIC_H_ */
