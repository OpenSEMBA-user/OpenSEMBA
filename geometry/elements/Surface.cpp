/*
 * Surface.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */
#include "Surface.h"

const double Surface::curvatureTolerance = 1e-12;

Surface::Surface(const LayerId layerId,
                 const MatId   matId)
:   Element(layerId, matId) {

}

Surface::Surface(const ElementId id,
                 const LayerId layerId,
                 const MatId   matId)
:   Element(id, layerId, matId) {

}

Surface::Surface(const Surface& rhs)
:   Element(rhs) {

}

Surface::~Surface() {

}

bool Surface::isContainedInPlane() const {
	return (isContainedInPlane(xy) ||
            isContainedInPlane(yz) ||
            isContainedInPlane(zx));
}

bool Surface::isContainedInPlane(
 const CartesianPlane plane) const {
	// Checks if any vertex lies out of the plane.
	for (uint i = 1; i < numberOfCoordinates(); i++) {
		if (!(*getV(i) - *getV(0)).isContainedInPlane(plane)) {
			return false;
	    }
	}
	return true;
}

CVecD3 Surface::getNormal() const {
	CVecD3 v0 = getVertex(1)->pos() - getVertex(0)->pos();
	CVecD3 v1 = getVertex(2)->pos() - getVertex(0)->pos();
	return (v0 ^ v1).normalize();
}

bool Surface::isRectangular() const {
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

void Surface::printInfo() const {
    cout << " --- Surface Info ---" << endl;
    cout << getNormal();
}
