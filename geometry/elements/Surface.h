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
    virtual ~Surface();
    Surface(
     const uint id_ = 0,
     const uint matId_ = 0,
     const uint layerId_ = 0);
    Surface&
     operator=(const Surface& rhs);
	virtual const Coordinate<double,3>*
	 getV(const unsigned int i) const = 0;
	bool isContainedInPlane(
	  const CartesianPlane plane) const;
	bool isContainedInPlane() const;
	virtual CVecD3 getNormal() const;
	virtual Element::Type getType() const {return SURFACE;}
	bool isRectangular() const;
	virtual void printInfo() const;
protected:
	static const double curvatureTolerance;
};

#endif /* SURFACE_H_ */
