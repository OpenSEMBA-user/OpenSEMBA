

#pragma once

#include "Cartesian.h"

namespace SEMBA {
namespace Math {
namespace Vector {

class Spherical {
public:
    Real radius;
    Real theta, phi;
    Spherical();
    Spherical(const Real theta, const Real phi);

    Spherical& operator=(const CVecR3& param);

    CVecR3 convertSphericalVectorField(const Real ar,
                                       const Real at,
                                       const Real ap) const;

    CVecR3 convertToCartesian() const;

    Real norm() const;

    void printInfo() const;
};

} /* namespace Vector */
} /* namespace Math */
} /* namespace SEMBA */

