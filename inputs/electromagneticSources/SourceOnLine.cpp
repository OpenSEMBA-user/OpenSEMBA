/*
 * Nodal.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#include "SourceOnLine.h"

SourceOnLine::SourceOnLine() {
	type_ = undefined;
	hardness_ = soft;
}

SourceOnLine::~SourceOnLine() {
	// TODO Auto-generated destructor stub
}

SourceOnLine::SourceOnLine(
 const Type& sourceType,
 const Hardness& sourceHardness,
 const vector<unsigned int>& elem,
 const Magnitude* magnitude) : EMSource(elem, magnitude) {
	type_ = sourceType;
    hardness_ = sourceHardness;
}

SourceOnLine&
SourceOnLine::operator=(const SourceOnLine &rhs) {
	if (this == &rhs) {
		return *this;
	}
	EMSource::operator=(rhs);
	type_ = rhs.type_;
	hardness_ = rhs.hardness_;
	return *this;
}

SourceOnLine::Type
SourceOnLine::getType() const {
	return type_;
}

SourceOnLine::Hardness
SourceOnLine::getHardness() const {
	return hardness_;
}

void
SourceOnLine::printInfo() const {
	cout<< " --- Nodal info --- " << endl;
	EMSource::printInfo();
	cout<< "Type: " << getTypeStr() << endl;
    cout<< "Hardness: " << getHardnessStr() << endl;
}

string
SourceOnLine::getTypeStr() const {
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
SourceOnLine::getHardnessStr() const {
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
