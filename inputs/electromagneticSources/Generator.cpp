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
 const Hardness& generatorHardness_,
 const vector<unsigned int>& elem_,
 const double spread_,
 const double delay_,
 const string& filename_) {
	type = generatorType_;
   hardness = generatorHardness_;
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

Generator::Hardness
Generator::getHardness() const {
	return hardness;
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
   cout<< "Hardness: " << getHardnessStr() << endl;
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

string
Generator::getHardnessStr() const {
	string res;
	switch (type) {
	case hard:
		res = "Hard";
		break;
	case current:
		res = "Soft";
		break;
	default:
		res = "Soft";
		break;
	}
	return res;
}
