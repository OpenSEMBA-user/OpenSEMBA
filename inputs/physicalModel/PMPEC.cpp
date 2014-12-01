/*
 * PMPEC.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMPEC.h"


// =============== PMPEC ======================================================
PMPEC::PMPEC() {

}

PMPEC::~PMPEC() {

}

PMPEC::PMPEC(const unsigned int id, const string name) {
	id_ = id;
	name_ = name;
}

PMPEC&
PMPEC::operator=(const PMPEC& rhs) {
	if (this == &rhs) {
		return *this;
	}
	id_ = rhs.id_;
	name_ = rhs.name_;
	return *this;
}

PMPEC&
PMPEC::operator=(const PMPredefined& rhs) {
	if (this == &rhs) {
		return *this;
	}
	id_ = rhs.getId();
	name_ = rhs.getName();
	return *this;
}

bool
PMPEC::isPEC() const {
	return true;
}

void
PMPEC::printInfo() const {
	cout << "--- PMPEC info ---" << endl;
	cout << "Id: " << id_ << endl;
	cout << "Name: " << name_ << endl;
	cout << "Perfect Electric Conductor" << endl;
}
