/*
 * Nodal.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#include "Nodal.h"

Nodal::Nodal() {
	type_ = undefined;
	hardness_ = soft;
}

Nodal::~Nodal() {
	// TODO Auto-generated destructor stub
}

Nodal::Nodal(
 const Type& sourceType,
 const Hardness& sourceHardness,
 const vector<unsigned int>& elem,
 const Magnitude* magnitude) : EMSource(elem, magnitude) {
	type_ = sourceType;
    hardness_ = sourceHardness;
}

Nodal&
Nodal::operator=(const Nodal &rhs) {
	if (this == &rhs) {
		return *this;
	}
	EMSource::operator=(rhs);
	type_ = rhs.type_;
	hardness_ = rhs.hardness_;
	return *this;
}

Nodal::Type
Nodal::getType() const {
	return type_;
}

Nodal::Hardness
Nodal::getHardness() const {
	return hardness_;
}

void
Nodal::printInfo() const {
	cout<< " --- Nodal info --- " << endl;
	EMSource::printInfo();
	cout<< "Type: " << getTypeStr() << endl;
    cout<< "Hardness: " << getHardnessStr() << endl;
}

string
Nodal::getTypeStr() const {
	string res;
	switch (type_) {
	case electricField:
		res = "Electric Field";
		break;
	case magneticField:
		res = "Magnetic Field";
		break;
	default:
		res = "Undefined";
		break;
	}
	return res;
}

string
Nodal::getHardnessStr() const {
	string res;
	switch (type_) {
	case hard:
		res = "Hard";
		break;
	case soft:
		res = "Soft";
		break;
	default:
		res = "Soft";
		break;
	}
	return res;
}
