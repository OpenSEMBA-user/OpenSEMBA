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
    static const Real pi = 3.14159265358979323846;  // Same as M_PI
}



#endif /* CONSTANTS_H_ */
