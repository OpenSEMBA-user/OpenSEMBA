/*
 * SphericalVector.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: luis
 */
#ifndef SPHERICALVECTOR_H_
#include "SphericalVector.h"
#endif

SphericalVector::SphericalVector() {
	radius = 0.0;
	theta = 0.0;
	phi = 0.0;
}

SphericalVector&
SphericalVector::operator=(const CartesianVector<double,3>& param) {
	radius = param.norm();
	theta = acos(param.val[2] / radius);
	phi = atan2(param.val[1], param.val[0]);
	return *this;
}
 
double
SphericalVector::norm() const {
	return radius;
}
 
CartesianVector<double,3>
SphericalVector::convertSphericalVectorField(
 const double ar,
 const double at,
 const double ap) const {
	double cost = cos(theta);
	double sint = sin(theta);
	double cosp = cos(phi);
	double sinp = sin(phi);
	CartesianVector<double,3> res;
	res(0) = cosp * (sint * ar + cost * at) - sinp * ap;
	res(1) = sinp * (sint * ar + cost * at) + cosp * ap;
	res(2) = cost * ar - sint * at;
	return res;
}

SphericalVector::SphericalVector(
 const double theta_,
 const double phi_) {
	radius = (double) 1.0;
	theta = theta_;
	phi = phi_;
}

CartesianVector<double, 3>
SphericalVector::convertToCartesian() const {
	CartesianVector<double,3> res;
    res(0) = radius * sin(theta) * cos(phi);
    res(1) = radius * sin(theta) * sin(phi);
    res(2) = radius * cos(theta);
    return res;
}

void
SphericalVector::printInfo() const {
	cout << "(" << radius << ", " << theta << ", " << phi << ") ";
}
