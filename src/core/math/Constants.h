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
 * constants.h
 *
 *  Created on: May 12, 2013
 *      Author: luis
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "Types.h"

#define SPEED_OF_LIGHT_SQ 8.98755179E+16
#define INVC0 3.33564095E-9
#define VACUUM_IMPEDANCE 3.76730313E+2
#define VACUUM_ADMITANCE 2.65441873E-3
#define INV4PIEPS0 8.98755179E+9
#define INV2PIEPS0 1.79751036E+10
#define SQRT2xC0 4.23970560000766E+08

#define INV4PI 0.079577471545947674

namespace Constants {
    static const Real eps0 = 8.85418782E-12;
    static const Real mu0 = 1.25663706E-6;
    static const Real c0 = 299792458.0;
    static const Real pi = 3.14159265358979323846;  // Same as Constants::pi
}



#endif /* CONSTANTS_H_ */
