/*
 * PMSMA.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMSMA.h"

PMSMA::PMSMA() {

}

PMSMA::~PMSMA() {

}

PMSMA::PMSMA(const unsigned int id, const string name) {
	id_ = id;
	name_ = name;
}

PMSMA&
PMSMA::operator=(const PMSMA& rhs) {
	if (this == &rhs) {
		return *this;
	}
	id_ = rhs.id_;
	name_ = rhs.name_;
	return *this;
}

PMSMA&
PMSMA::operator=(const PMPredefined& rhs) {
	if (this == &rhs) {
		return *this;
	}
	id_ = rhs.getId();
	name_ = rhs.getName();
	return *this;
}

bool
PMSMA::isSMA() const {
	return true;
}

void
PMSMA::printInfo() const {
	cout << "--- PMSMA info ---" << endl;
	cout << "Id: " << id_ << endl;
	cout << "Name: " << name_ << endl;
	cout << "Silver-Muller Absorbing" << endl;
}
