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

#include "Real.h"

namespace SEMBA {
namespace Math {
namespace Util {

bool equal(const Real lhs, const Real rhs,
           Real rel, const Real tol) {
    if (rel == 0.0) {
        rel = abs(lhs+rhs);
    }
    if ((abs(lhs) <= epsilon) && (abs(rhs) <= epsilon)) {
        return true;
    } else if ((abs(lhs) <= epsilon) || (abs(rhs) <= epsilon)) {
        return false;
    } else if (abs(lhs-rhs) <= tol*rel) {
        return true;
    }
    return false;
}

bool notEqual(const Real lhs, const Real rhs,
              Real rel, const Real tol) {
    return !equal(lhs, rhs, rel, tol);
}

bool lower(const Real lhs, const Real rhs,
           Real rel, const Real tol) {
    if(equal(lhs, rhs, rel, tol)) {
        return false;
    }
    return lhs < rhs;
}

bool lowerEqual(const Real lhs, const Real rhs,
                Real rel, const Real tol) {
    return !lower(rhs, lhs, rel, tol);
}

bool greater(const Real lhs, const Real rhs,
             Real rel, const Real tol) {
    return lower(rhs, lhs, rel, tol);
}

bool greaterEqual(const Real lhs, const Real rhs,
                  Real rel, const Real tol) {
    return !lower(lhs, rhs, rel, tol);
}

Real ceil(const Real val, const Real rel) {
    Real low = floor(val);
    if (val <= (low + rel)) {
        return low;
    } else {
        return std::ceil(val);
    }
}

Real round(Real v) {
    Real dist = v - floor(v);
    if (greaterEqual(dist, 0.5)) {
        return ceil(v);
    } else {
        return floor(v);
    }
}

} /* namespace Constant */
} /* namespace Math */
} /* namespace SEMBA */
