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

#include "Types.h"

namespace SEMBA {
namespace Math {
namespace Constants {

const Real SPEED_OF_LIGHT_SQ = 8.98755179E+16;
const Real INVC0             = 3.33564095E-9;
const Real VACUUM_IMPEDANCE  = 3.76730313E+2;
const Real VACUUM_ADMITANCE  = 2.65441873E-3;
const Real INV4PIEPS0        = 8.98755179E+9;
const Real INV2PIEPS0        = 1.79751036E+10;
const Real SQRT2xC0          = 4.23970560000766E+08;

const Real INV4PI            = 0.079577471545947674;

const Real eps0 = 8.85418782E-12;
const Real mu0  = 1.25663706E-6;
const Real c0   = 299792458.0;
const Real pi   = 3.14159265358979323846;  // Same as M_PI
const Real pi_2 = 1.57079632679489661923;  // Same as M_PI_2

enum CartesianPlane {
    xy = 0,
    yz = 1,
    zx = 2
};

enum CartesianAxis {
    x = 0,
    y = 1,
    z = 2
};

enum CartesianBound {
    L = 0,
    U = 1
};

} /* namespace Constants */
} /* namespace Math */
} /* namespace SEMBA */

