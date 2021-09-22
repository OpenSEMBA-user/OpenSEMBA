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

#include <cmath>
#include <limits>

#include "math/Types.h"

namespace SEMBA {
namespace Math {
namespace Util {

const Real epsilon = std::numeric_limits<Real>::epsilon()*1.0e2;
const Real tolerance = 1e-10;

bool equal       (const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);
bool notEqual    (const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);
bool lower       (const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);
bool lowerEqual  (const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);
bool greater     (const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);
bool greaterEqual(const Real lhs, const Real rhs,
                  Real rel = 0.0, const Real tol = tolerance);

Real sign(Real val);
Real ceil(const Real v, Real rel = 0.0);
Real round(Real v);

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */

