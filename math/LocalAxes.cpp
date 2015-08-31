/*
 * LocalAxes.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#include "LocalAxes.h"

LocalAxes::LocalAxes() {
}

LocalAxes::LocalAxes(CVecR3 eulerAngles, CVecR3 origin) {
    eulerAngles_ = eulerAngles;
    origin_ = origin;
}

LocalAxes::~LocalAxes() {

}

const CVecR3 LocalAxes::getEulerAngles() const {
    return eulerAngles_;
}

const CVecR3 LocalAxes::getOrigin() const {
    return origin_;
}

MatR33 LocalAxes::getTransformationMatrix() const {
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

MatR33 LocalAxes::convertToGlobal(const MatR33& local) const {
    MatR33 transformation = getTransformationMatrix();
    MatR33 transformationTransposed = getTransformationMatrix().transpose();
    MatR33 global = transformation * local * transformationTransposed;
    return global;
}

CVecR3 LocalAxes::convertToGlobal(const CVecR3& local) const {
    MatR33 transformationTransposed = getTransformationMatrix().transpose();
    CVecR3 global = transformationTransposed * local + origin_;
    return global;
}
