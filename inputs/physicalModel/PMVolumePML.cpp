/*
 * PMVolumePML.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolumePML.h"


PMVolumePML::PMVolumePML() {
	name_ = "";
	id_ = 0;
	rPermittivity = 1.0;
	rPermeability = 1.0;
	impedance = sqrt(double(rPermeability*mu0)
	 / double(rPermittivity*eps0));
	admitance = 1 / impedance;
	for (unsigned int i = 0; i < 3; i++) {
		direction[i] = none;
	}
}

PMVolumePML::PMVolumePML(const unsigned int id, const string& name) {
	name_ = name;
	id_ = id;
	rPermittivity = 1.0;
	rPermeability = 1.0;
	impedance = sqrt(double(rPermeability*mu0)
	 / double(rPermittivity*eps0));
	admitance = 1 / impedance;
	for (unsigned int i = 0; i < 3; i++) {
		direction[i] = none;
	}
}

PMVolumePML::PMVolumePML(
 const unsigned int id,
 const Direction direction_[3],
 const BoxD3& bound_) {
	id_ = id;
	name_ = "PML";
	bound.first = bound_.getMin();
	bound.second = bound_.getMax();
	for (unsigned int i = 0; i < 3; i++) {
		direction[i] = direction_[i];
		if (direction[i] == plus) {
			name_ += "+";
		} else if (direction[i] == minus) {
			name_ += "-";
		} else {
			name_ += "0";
		}
	}
	rPermittivity = 1.0;
	rPermeability = 1.0;
	impedance = sqrt(double(rPermeability*mu0)
	 / double(rPermittivity*eps0));
	admitance = 1 / impedance;
}

PMVolumePML::~PMVolumePML() {

}

const PMVolumePML::Direction*
PMVolumePML::getDirection() const {
	return direction;
}

PMVolumePML&
PMVolumePML::operator=(const PMVolumePML &rhs) {
	if (this == &rhs) {
		return *this;
	}
	id_ = rhs.id_;
	name_ = rhs.name_;
	rPermittivity = rhs.rPermittivity;
	rPermeability = rhs.rPermeability;
	impedance = rhs.impedance;
	admitance = rhs.admitance;
	bound = rhs.bound;
	for (unsigned int i = 0; i < 3; i++) {
		direction[i] = rhs.direction[i];
	}
	return *this;
}

bool
PMVolumePML::isPML() const {
	return true;
}

void
PMVolumePML::printInfo() const {
	cout << "--- PMVolume info ---" << endl;
	cout << "Id: " << id_ << endl;
	cout << "Name: " << name_ << endl;
	cout << "Type: " << "PML" << endl;
}

PMVolumePML::Orientation
PMVolumePML::getOrientation() const {
	for (unsigned int i = 0; i < 3; i++) {
		unsigned int first = i;
		unsigned int second = (i+1) % 3;
		unsigned int third = (i+2) % 3;
		// Is uniaxial?.
		bool isUniaxial =
		 (direction[first] == plus || direction[first] == minus)
		 && direction[second] == none
		 && direction[third] == none;
		if (isUniaxial) {
			if (i == x) {
				return PMLx;
			} else if (i == y) {
				return PMLy;
			} else if (i == z) {
				return PMLz;
			}
		}
		// Is biaxial?
		bool isBiaxial =
		 (direction[first] == plus || direction[first] == minus)
		 && (direction[first] == plus || direction[first] == minus)
		 && direction[third] == none;
		if (isBiaxial) {
			if (i == x) {
				return PMLxy;
			} else if (i == y) {
				return PMLyz;
			} else if (i == z) {
				return PMLzx;
			}
		}
	}
	if (direction[0] != none
	 && direction[1] != none
	 && direction[2] != none) {
		return PMLxyz;
	}
	return undefined;
}

bool
PMVolumePML::isUniaxial() const {
	Orientation orientation = getOrientation();
	return (
	 orientation == PMLx
	 || orientation == PMLy
	 || orientation == PMLz);
}

bool
PMVolumePML::isBiaxial() const {
	Orientation orientation = getOrientation();
	return (
	 orientation == PMLxy
	 || orientation == PMLyz
	 || orientation == PMLzx);
}

bool
PMVolumePML::isTriaxial() const {
	return (getOrientation() == PMLxyz);
}

const pair<CVecD3, CVecD3>&
PMVolumePML::getBound() const {
	return bound;
}

CVecD3
PMVolumePML::getWidth() const {
	CVecD3 res;
	for (unsigned int i = 0; i < 3; i++) {
		res(i) = abs(bound.second(i) - bound.first(i));
	}
	return res;
}

unsigned int
PMVolumePML::getFirstOrientationIndex() const {
	Orientation orientation = getOrientation();
	switch (orientation) {
	case PMLx:
		return 0;
	case PMLy:
		return 1;
	case PMLz:
		return 2;
	case PMLxy:
		return 0;
	case PMLyz:
		return 1;
	case PMLzx:
		return 2;
	case PMLxyz:
		return 0;
	default:
		cerr << "ERROR @ PMVolumePML " << endl;
		cerr << "Unreckognized orientation" << endl;
		assert(false);
		exit(-1);
	}
}

CVecD3
PMVolumePML::getPMLBeginningPosition() const {
	CVecD3 res;
	for (unsigned int i = 0; i < 3; i++) {
		if (direction[i] == plus) {
			res(i) = bound.first(i);
		} else if (direction[i] == minus) {
			res(i) = bound.second(i);
		}
	}
	return res;
}

void
PMVolumePML::check() const {
	// Checks validity of parameters.
	if (id_ == 0) {
		cerr << "ERROR @ PMVolumePML ctor" << endl;
		cerr << "Invalid id value." << endl;
		exit(PHYSICALMODEL_ERROR);
	}
	assert(
	 !(direction[x] == none
	 && direction[y] == none
	 && direction[z] == none));
}
