/*
 * Surface.h
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#ifndef SURFACE_H_
#define SURFACE_H_

#include "Element.h"

using namespace std;

class Surface : public Element {
public:
	// Normal at the center of the surface.
	virtual const Coordinate<double,3>*
	 getV(const unsigned int i) const = 0;
	Surface();
	virtual ~Surface();
	bool
	 isContainedInPlane(
	  const CartesianPlane plane) const;
	bool
	 isContainedInPlane() const;
	CartesianVector<double,3>
	 getNormal() const;
	bool
	 isRectangular() const;
	virtual void
	 printInfo() const = 0;
protected:
	CVecD3 normal;
#ifdef __GXX_EXPERIMENTAL_CXX0X__
	static constexpr double curvatureTolerance = 1e-12;
#else
	static const double curvatureTolerance = 1e-12;
#endif
};

#endif /* SURFACE_H_ */
