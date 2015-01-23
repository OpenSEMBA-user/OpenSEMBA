/*
 * Surface.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */
#include "Surface.h"

const double Surface::curvatureTolerance = 1e-12;


Surface::Surface(
 const CVecD3& normal_,
 const uint id_,
 const uint matId_,
 const uint layerId_) : Element(id_, matId_, layerId_) {
    normal = normal_;
}


Surface::Surface() {

}

Surface::~Surface() {

}

bool
Surface::isContainedInPlane() const {
	return (isContainedInPlane(xy)
	 || isContainedInPlane(yz) || isContainedInPlane(zx));
}

bool
Surface::isContainedInPlane(
 const CartesianPlane plane) const {
	// Checks if any vertex lies out of the plane.
	for (uint i = 1; i < numberOfCoordinates(); i++) {
		if (!(*getV(i) - *getV(0)).isContainedInPlane(plane)) {
			return false;
	    }
	}
	return true;
}

CartesianVector<double, 3>
Surface::getNormal() const {
	return normal;
}

bool
Surface::isRectangular() const {
	if (numberOfCoordinates() != 4 || numberOfFaces() != 4) {
		return false;
	}
	for (uint f = 0; f < 4; f++) {
		CVecD3 p0 = getSideVertex(f % 4, 0)->pos();
		CVecD3 p1 = getSideVertex(f % 4, 1)->pos();
		CVecD3 p2 = getSideVertex((f+1) % 4, 1)->pos();
		double sProd = (p2-p1).dot(p1 - p0);
		if (sProd > tolerance) {
			return false;
		}
	}
	return true;
}

Surface&
Surface::operator =(const Surface& rhs) {
    if (&rhs == this) {
        return *this;
    }
    Element::operator=(rhs);
    normal = rhs.normal;
    return *this;
}
