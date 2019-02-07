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

#ifndef SEMBA_MATH_AXIS_LOCAL_H_
#define SEMBA_MATH_AXIS_LOCAL_H_

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

    void printInfo() const;

private:
    CVecR3 eulerAngles_; // Euler angles in radians.
    CVecR3 origin_;
};

} /* namespace Axis */
} /* namespace Math */
} /* namespace SEMBA */

#endif /* SEMBA_MATH_AXIS_LOCAL_H_ */
