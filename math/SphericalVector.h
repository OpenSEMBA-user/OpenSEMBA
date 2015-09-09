/*
 * SphericalVector.h
 *
 *  Created on: Sep 1, 2012
 *      Author: luis
 */
#ifndef SPHERICALVECTOR_H_
#define SPHERICALVECTOR_H_

#include <cmath>
#include <iostream>
using namespace std;

#include "CartesianVector.h"

class SphericalVector {
public:
	Real radius;
	Real theta, phi;
	SphericalVector();
	SphericalVector(const Real theta, const Real phi);
	SphericalVector& operator=(const CVecR3& param);
	CVecR3 convertSphericalVectorField(
	  const Real ar,
	  const Real at,
	  const Real ap) const;
	CVecR3 convertToCartesian() const;
	Real norm() const;
	void printInfo() const;
};

#endif /* SPHERICALVECTOR_H_ */
