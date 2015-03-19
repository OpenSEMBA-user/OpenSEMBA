#ifndef REALUTILS_H_
#define REALUTILS_H_

#include <cmath>
using namespace std;

#include "Types.h"

namespace MathUtils {
    const Real tolerance = 1.0e-7;

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
