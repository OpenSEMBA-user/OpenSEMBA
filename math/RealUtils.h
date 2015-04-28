#ifndef REALUTILS_H_
#define REALUTILS_H_

#include <cmath>
#include <limits>
using namespace std;

#include "Types.h"

namespace MathUtils {
    const Real epsilon = numeric_limits<Real>::epsilon()*1.0e2;
    const Real tolerance = 1e-10;

    bool equal        (const Real lhs, const Real rhs,
                       Real rel = 0.0, const Real tol = tolerance);
    bool notEqual     (const Real lhs, const Real rhs,
                       Real rel = 0.0, const Real tol = tolerance);
    bool lower        (const Real lhs, const Real rhs,
                       Real rel = 0.0, const Real tol = tolerance);
    bool lowerEqual   (const Real lhs, const Real rhs,
                       Real rel = 0.0, const Real tol = tolerance);
    bool greather     (const Real lhs, const Real rhs,
                       Real rel = 0.0, const Real tol = tolerance);
    bool greatherEqual(const Real lhs, const Real rhs,
                       Real rel = 0.0, const Real tol = tolerance);
}


#endif
