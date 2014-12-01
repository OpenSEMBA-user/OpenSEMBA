/*
 * Generator.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#include "Generator.h"

Generator::Generator() {
	type = undefined;
}

Generator::~Generator() {
	// TODO Auto-generated destructor stub
}

Generator::Generator(
 const Type& generatorType_,
 const vector<unsigned int>& elem_,
 const double spread_,
 const double delay_,
 const string& filename_) {
	type = generatorType_;
	elem = elem_;
	spread = spread_;
	delay = delay_;
	filename = filename_;
}

Generator&
Generator::operator=(const Generator &rhs) {
	if (this == &rhs) {
		return *this;
	}
	type = rhs.type;
	elem = rhs.elem;
	spread = rhs.spread;
	delay = rhs.delay;
	filename = rhs.filename;
	return *this;
}

Generator::Type
Generator::getType() const {
	return type;
}

void
Generator::printInfo() const {
	cout<< " --- Generator info --- " << endl;
	cout<< " - Assigned on " << elem.size() << " nodes:" << endl;
	for (uint i = 0; i < elem.size(); i++) {
		cout<< elem[i] << " ";
	}
	cout<<endl;
	cout<< "Type: " << getTypeStr() << endl;
	printMagnitude();
}

string
Generator::getTypeStr() const {
	string res;
	switch (type) {
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
