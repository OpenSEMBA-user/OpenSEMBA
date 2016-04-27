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

#include "Local.h"

namespace SEMBA {
namespace Math {
namespace Axis {

Local::Local() {
}

Local::Local(CVecR3 eulerAngles, CVecR3 origin) {
    eulerAngles_ = eulerAngles;
    origin_ = origin;
}

Local::~Local() {

}

const CVecR3 Local::getEulerAngles() const {
    return eulerAngles_;
}

const CVecR3 Local::getOrigin() const {
    return origin_;
}

MatR33 Local::getTransformationMatrix() const {
    MatR33 res;
    const Real phi = eulerAngles_(0);
    const Real theta = eulerAngles_(1);
    const Real psi = eulerAngles_(2);

    res(0,0) =   cos(psi)*cos(phi) - sin(psi)*cos(theta)*sin(phi);
    res(0,1) =   cos(psi)*sin(phi) + sin(psi)*cos(theta)*cos(phi);
    res(0,2) =   sin(psi)*sin(theta);

    res(1,0) = - sin(psi)*cos(phi) - cos(psi)*cos(theta)*sin(phi);
    res(1,1) = - sin(psi)*sin(phi) + cos(psi)*cos(theta)*cos(phi);
    res(1,2) =   cos(psi)*sin(theta);

    res(2,0) =   sin(theta)*sin(phi);
    res(2,1) = - sin(theta)*cos(phi);
    res(2,2) =   cos(theta);

    return res;
}

MatR33 Local::convertToGlobal(const MatR33& local) const {
    MatR33 transformation = getTransformationMatrix();
    MatR33 transformationTransposed = getTransformationMatrix().transpose();
    MatR33 global = transformation * local * transformationTransposed;
    return global;
}

CVecR3 Local::convertToGlobal(const CVecR3& local) const {
    MatR33 transformation = getTransformationMatrix();
    CVecR3 global = transformation * local + origin_;
    return global;
}

void Local::printInfo() const {
    std::cout << " --- Local Axes Info --- " << std::endl;
    std::cout << "Euler angles: " << eulerAngles_ << std::endl;
    std::cout << "Origin:       " << origin_ << std::endl;
}

} /* namespace Axis */
} /* namespace Math */
} /* namespace SEMBA */
