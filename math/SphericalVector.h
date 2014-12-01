/*
 * SphericalVector.h
 *
 *  Created on: Sep 1, 2012
 *      Author: luis
 */
#ifndef SPHERICALVECTOR_H_
#define SPHERICALVECTOR_H_

#include <iostream>
#include <cmath>
#include "CartesianVector.h"

using namespace std;

class SphericalVector {
public:
	double radius;
	double theta, phi;
	SphericalVector();
	SphericalVector(const double theta, const double phi);
	SphericalVector&
	 operator=(const CartesianVector<double,3>& param);
	CartesianVector<double,3>
	 convertSphericalVectorField(
	  const double ar,
	  const double at,
	  const double ap) const;
	CartesianVector<double,3>
	 convertToCartesian() const;
	double
	 norm() const;
	void
	 printInfo() const;
};

#endif /* SPHERICALVECTOR_H_ */
