/*
 * Volume.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#include "Volume.h"

Volume::Volume() {

}

Volume::Volume(const uint id_, const uint matId_, const uint layerId_) :
    Element(id_, matId_, layerId_) {
}

Volume::~Volume() {

}

unsigned int
Volume::getFaceNumber(const Surface* surf) const {
	// Checks each face. Order is not important.
	for (unsigned int f = 0; f < numberOfFaces(); f++) {
		unsigned int vPresent = 0;
		for (unsigned int i = 0; i < surf->numberOfVertices(); i++) {
			for (unsigned int j = 0; j < surf->numberOfVertices(); j++) {
				if (surf->getVertex(j) == getSideVertex(f, i)) {
					vPresent++;
				}
			}
			if (vPresent == surf->numberOfVertices()) {
				return f;
			}
		}
	}
	// If face was not found, shows error message.
	cerr << "ERROR @ VolumeElement::getFaceNumber()"      << endl;
	cerr << "Surf " << surf->getId() << " is not part of VolumeElement "
	     << id << endl;
	cerr << "Volume: " << endl;
	printInfo();
	cerr << "Surface: " << endl;
	surf->printInfo();
	exit(ELEMENT_ERROR);
}

CartesianVector<double,3>
Volume::sideNormal(const unsigned int f) const {
	CartesianVector<double,3> vec1, vec2, res;
	vec1 = *getSideVertex(f,1) - *getSideVertex(f,0);
	vec2 = *getSideVertex(f,2) - *getSideVertex(f,0);
	res = (vec1 ^ vec2).normalize();
	return res;
}

bool
Volume::isLocalFace(
 const unsigned int f, const Surface& surf) const {
	return sideNormal(f) == surf.getNormal();
}

BoxD3
Volume::getBoundOfFace(const unsigned int face) const {
   BoxD3 res;
   for (unsigned int i = 0; i < numberOfSideCoordinates(); i++) {
      res << getSideV(face,i)->pos();
   }
   return res;
}

bool
Volume::isFaceContainedInPlane(
      const uint face, const CartesianPlane plane) const {
   BoxD3 box = getBoundOfFace(face);
   CVecD3 vec = box.getMax() - box.getMin();
   return vec.isContainedInPlane(plane);
}
