// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * PMVolumePML.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolumePML.h"

PMVolumePML::PMVolumePML(const MatId id, const string& name)
: PMVolume(id, name){
	for (UInt i = 0; i < 3; i++) {
		direction[i] = none;
	}
}

PMVolumePML::PMVolumePML(
 const MatId id,
 const Direction direction_[3],
 const BoxR3& bound_) : PMVolume(id, "PML"){
	bound.first = bound_.getMin();
	bound.second = bound_.getMax();
	for (UInt i = 0; i < 3; i++) {
		direction[i] = direction_[i];
		if (direction[i] == plus) {
			setName(getName() + "+");
		} else if (direction[i] == minus) {
			setName(getName() + "-");
		} else {
		    setName(getName() + "0");
		}
	}
}

PMVolumePML::~PMVolumePML() {

}

const PMVolumePML::Direction*
PMVolumePML::getDirection() const {
	return direction;
}

void
PMVolumePML::printInfo() const {
	cout << "--- PMVolume info ---" << endl;
	PMVolume::printInfo();
	cout << "Type: " << "PML" << endl;
}

PMVolumePML::Orientation
PMVolumePML::getOrientation() const {
	for (UInt i = 0; i < 3; i++) {
		UInt first = i;
		UInt second = (i+1) % 3;
		UInt third = (i+2) % 3;
		// Is uniaxial?.
		bool isUniaxial =
		 (direction[first] == plus || direction[first] == minus)
		 && direction[second] == none
		 && direction[third] == none;
		if (isUniaxial) {
			if (i == x) {
				return Orientation::PMLx;
			} else if (i == y) {
				return Orientation::PMLy;
			} else if (i == z) {
				return Orientation::PMLz;
			}
		}
		// Is biaxial?
		bool isBiaxial =
		 (direction[first] == plus || direction[first] == minus)
		 && (direction[first] == plus || direction[first] == minus)
		 && direction[third] == none;
		if (isBiaxial) {
			if (i == x) {
				return Orientation::PMLxy;
			} else if (i == y) {
				return Orientation::PMLyz;
			} else if (i == z) {
				return Orientation::PMLzx;
			}
		}
	}
	if (direction[0] != none
	 && direction[1] != none
	 && direction[2] != none) {
		return Orientation::PMLxyz;
	}
	return Orientation::undefined;
}

bool PMVolumePML::isUniaxial() const {
	Orientation orientation = getOrientation();
	return (
	 orientation == Orientation::PMLx
	 || orientation == Orientation::PMLy
	 || orientation == Orientation::PMLz);
}

bool PMVolumePML::isBiaxial() const {
	Orientation orientation = getOrientation();
	return (
	 orientation == Orientation::PMLxy
	 || orientation == Orientation::PMLyz
	 || orientation == Orientation::PMLzx);
}

bool PMVolumePML::isTriaxial() const {
	return (getOrientation() == Orientation::PMLxyz);
}

const pair<CVecR3, CVecR3>&
PMVolumePML::getBound() const {
	return bound;
}

CVecR3 PMVolumePML::getWidth() const {
	CVecR3 res;
	for (UInt i = 0; i < 3; i++) {
		res(i) = abs(bound.second(i) - bound.first(i));
	}
	return res;
}

UInt PMVolumePML::getFirstOrientationIndex() const {
	Orientation orientation = getOrientation();
	switch (orientation) {
	case Orientation::PMLx:
		return 0;
	case Orientation::PMLy:
		return 1;
	case Orientation::PMLz:
		return 2;
	case Orientation::PMLxy:
		return 0;
	case Orientation::PMLyz:
		return 1;
	case Orientation::PMLzx:
		return 2;
	case Orientation::PMLxyz:
		return 0;
	default:
	    return 0;
	}
}

CVecR3 PMVolumePML::getPMLBeginningPosition() const {
	CVecR3 res;
	for (UInt i = 0; i < 3; i++) {
		if (direction[i] == plus) {
			res(i) = bound.first(i);
		} else if (direction[i] == minus) {
			res(i) = bound.second(i);
		}
	}
	return res;
}
