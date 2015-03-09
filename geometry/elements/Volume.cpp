/*
 * Volume.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#include "Volume.h"

Volume::Volume(const LayerId layerId,
               const MatId   matId)
:   Element(layerId, matId) {

}

Volume::Volume(const ElementId id,
               const LayerId layerId,
               const MatId   matId)
:   Element(id, layerId, matId) {

}

Volume::Volume(const Volume& rhs)
:   Element(rhs) {

}

Volume::~Volume() {

}

bool Volume::isLocalFace(const uint f, const Surface& surf) const {
	return sideNormal(f) == surf.getNormal();
}

bool Volume::isFaceContainedInPlane(const uint face,
                                    const CartesianPlane plane) const {
   BoxD3 box = getBoundOfFace(face);
   CVecD3 vec = box.getMax() - box.getMin();
   return vec.isContainedInPlane(plane);
}

CVecD3 Volume::sideNormal(const uint f) const {
	CartesianVector<double,3> vec1, vec2, res;
	vec1 = *getSideVertex(f,1) - *getSideVertex(f,0);
	vec2 = *getSideVertex(f,2) - *getSideVertex(f,0);
	res = (vec1 ^ vec2).normalize();
	return res;
}

BoxD3 Volume::getBoundOfFace(const uint face) const {
   BoxD3 res;
   for (uint i = 0; i < numberOfSideCoordinates(); i++) {
      res << getSideV(face,i)->pos();
   }
   return res;
}

uint Volume::getFaceNumber(const Surface* surf) const {
	// Checks each face. Order is not important.
	for (uint f = 0; f < numberOfFaces(); f++) {
		uint vPresent = 0;
		for (uint i = 0; i < surf->numberOfVertices(); i++) {
			for (uint j = 0; j < surf->numberOfVertices(); j++) {
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
	     << getId() << endl;
	cerr << "Volume: " << endl;
	printInfo();
	cerr << "Surface: " << endl;
	surf->printInfo();
	exit(ELEMENT_ERROR);
}
