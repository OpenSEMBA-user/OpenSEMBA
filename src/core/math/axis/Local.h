

#pragma once

#include "math/vector/Cartesian.h"
#include "math/matrix/Static.h"

namespace SEMBA {
namespace Math {
namespace Axis {

class Local {
public:
    Local();
    Local(CVecR3 eulerAngles, CVecR3 origin = CVecR3());
    virtual ~Local();

    MatR33 getTransformationMatrix() const;
    const CVecR3 getEulerAngles() const;
    const CVecR3 getOrigin() const;

    MatR33 convertToGlobal(const MatR33& local) const;
    CVecR3 convertToGlobal(const CVecR3& local) const;

private:
    CVecR3 eulerAngles_; // Euler angles in radians.
    CVecR3 origin_;
};

} /* namespace Axis */
} /* namespace Math */
} /* namespace SEMBA */

