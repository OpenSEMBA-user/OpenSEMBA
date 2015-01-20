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
    Surface();
    virtual ~Surface();
    Surface(
     const CVecD3& normal_,
     const uint id_ = 0,
     const uint matId_ = 0,
     const uint layerId_ = 0);
    Surface&
     operator=(const Surface& rhs);
	virtual const Coordinate<double,3>*
	 getV(const unsigned int i) const = 0;
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
	static const double curvatureTolerance = 1e-12;
};

#endif /* SURFACE_H_ */
