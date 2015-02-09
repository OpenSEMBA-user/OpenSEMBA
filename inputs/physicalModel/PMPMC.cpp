/*
 * PMPMC.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMPMC.h"

PMPMC::PMPMC() {

}

PMPMC::~PMPMC() {

}

PMPMC::PMPMC(const unsigned int id, const string name) {
	id_ = id;
	name_ = name;
}

PMPMC&
PMPMC::operator=(const PMPMC& rhs) {
	if (this == &rhs) {
		return *this;
	}
	id_ = rhs.id_;
	name_ = rhs.name_;
	return *this;
}

PMPMC&
PMPMC::operator=(const PMPredefined& rhs) {
	if (this == &rhs) {
		return *this;
	}
	id_ = rhs.getId();
	name_ = rhs.getName();
	return *this;
}

bool
PMPMC::isPMC() const {
	return true;
}

void
PMPMC::printInfo() const {
	cout << "--- PMPMC info ---" << endl;
	cout << "Id: " << id_ << endl;
	cout << "Name: " << name_ << endl;
	cout << "Perfect Magnetic Conductor" << endl;
}
