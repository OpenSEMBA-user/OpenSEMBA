

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

