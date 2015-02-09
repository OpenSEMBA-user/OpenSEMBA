/*
 * Generator.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#include "Generator.h"

Generator::Generator() {
	type_ = undefined;
}

Generator::~Generator() {
	// TODO Auto-generated destructor stub
}

Generator::Generator(
 const Type& generatorType,
 const vector<unsigned int>& elem,
 const Magnitude* magnitude) : EMSource(elem, magnitude) {
	type_ = generatorType;
}

Generator&
Generator::operator=(const Generator &rhs) {
	if (this == &rhs) {
		return *this;
	}
	EMSource::operator=(rhs);
	type_ = rhs.type_;
	return *this;
}

Generator::Type
Generator::getType() const {
	return type_;
}

void
Generator::printInfo() const {
	cout<< " --- Generator info --- " << endl;
	EMSource::printInfo();
	cout<< "Type: " << getTypeStr() << endl;
}

string
Generator::getTypeStr() const {
	string res;
	switch (type_) {
	case voltage:
		res = "Voltage";
		break;
	case current:
		res = "Current";
		break;
	default:
		res = "Undefined";
		break;
	}
	return res;
}
