/*
 * PMMultiportRLC.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMMultiportRLC.h"

PMMultiportRLC::PMMultiportRLC() {
	type_ = undefined;
	R_ = 0.0;
	L_ = 0.0;
	C_ = 0.0;
}

PMMultiportRLC::PMMultiportRLC(
 const uint id,
 const string name,
 const PMMultiport::Type type,
 const double resistance,
 const double inductance,
 const double capacitance) {
	id_ = id;
	name_ = name;
	type_ = type;
	R_ = resistance;
	L_ = inductance;
	C_ = capacitance;
}

PMMultiportRLC::~PMMultiportRLC() {

}

double
 PMMultiportRLC::getR() const {
	return R_;
}

double
 PMMultiportRLC::getL() const {
	return L_;
}

double
 PMMultiportRLC::getC() const {
	return C_;
}

void
PMMultiportRLC::printInfo() const {
	cout<< " --- Multiport RLC Info --- " << endl
		<< " Id: " << id_ << endl
		<< " Name: " << name_ << endl
		<< " Type: " << getTypeStr() << endl
		<< " Resistance: " << R_ << endl
		<< " Inductance: " << L_ << endl
		<< " Capacitance: " << C_ << endl;
}
