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

#include "Spherical.h"

#include <cmath>
#include <iostream>

namespace SEMBA {
namespace Math {
namespace Vector {

Spherical::Spherical() {
    radius = 0.0;
    theta = 0.0;
    phi = 0.0;
}

Spherical::Spherical(const Real theta_, const Real phi_) {
    radius = (Real) 1.0;
    theta = theta_;
    phi = phi_;
}

Spherical& Spherical::operator=(const CVecR3& param) {
    radius = param.norm();
    theta = acos(param.val[2] / radius);
    phi = atan2(param.val[1], param.val[0]);
    return *this;
}

CVecR3 Spherical::convertSphericalVectorField(const Real ar,
                                              const Real at,
                                              const Real ap) const {
    Real cost = cos(theta);
    Real sint = sin(theta);
    Real cosp = cos(phi);
    Real sinp = sin(phi);
    CVecR3 res;
    res(0) = cosp * (sint * ar + cost * at) - sinp * ap;
    res(1) = sinp * (sint * ar + cost * at) + cosp * ap;
    res(2) = cost * ar - sint * at;
    return res;
}

CVecR3 Spherical::convertToCartesian() const {
    CVecR3 res;
    res(0) = radius * sin(theta) * cos(phi);
    res(1) = radius * sin(theta) * sin(phi);
    res(2) = radius * cos(theta);
    return res;
}

Real Spherical::norm() const {
    return radius;
}

void Spherical::printInfo() const {
    std::cout << "(" << radius << ", " << theta << ", " << phi << ") ";
}

} /* namespace Vector */
} /* namespace Math */
} /* namespace SEMBA */
