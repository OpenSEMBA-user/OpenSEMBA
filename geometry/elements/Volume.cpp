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

bool Volume::isLocalFace(const UInt f, const Surface& surf) const {
	return sideNormal(f) == surf.getNormal();
}

bool Volume::isFaceContainedInPlane(const UInt face,
                                    const CartesianPlane plane) const {
   BoxR3 box = getBoundOfFace(face);
   CVecR3 vec = box.getMax() - box.getMin();
   return vec.isContainedInPlane(plane);
}

CVecR3 Volume::sideNormal(const UInt f) const {
	CartesianVector<Real,3> vec1, vec2, res;
	vec1 = *getSideVertex(f,1) - *getSideVertex(f,0);
	vec2 = *getSideVertex(f,2) - *getSideVertex(f,0);
	res = (vec1 ^ vec2).normalize();
	return res;
}

BoxR3 Volume::getBoundOfFace(const UInt face) const {
   BoxR3 res;
   for (UInt i = 0; i < numberOfSideCoordinates(); i++) {
      res << getSideV(face,i)->pos();
   }
   return res;
}

UInt Volume::getFaceNumber(const Surface* surf) const {
	// Checks each face. Order is not important.
	for (UInt f = 0; f < numberOfFaces(); f++) {
		UInt vPresent = 0;
		for (UInt i = 0; i < surf->numberOfVertices(); i++) {
			for (UInt j = 0; j < surf->numberOfVertices(); j++) {
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
