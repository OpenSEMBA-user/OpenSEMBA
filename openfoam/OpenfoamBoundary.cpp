/*
 * OpenfoamBoundary.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: luis
 */

#include "OpenfoamBoundary.h"

OpenfoamBoundary::OpenfoamBoundary() {
	id_ = 0;
	nFaces_ = 0;
	startFace_ = 0;
}

OpenfoamBoundary::OpenfoamBoundary(
 const uint& id,
 const string& name,
 const uint& nFaces,
 const uint& startFace) {
	id_ = id;
	name_ = name;
	nFaces_ = nFaces;
	startFace_ = startFace;
}

OpenfoamBoundary::~OpenfoamBoundary() {
	// TODO Auto-generated destructor stub
}

OpenfoamBoundary&
OpenfoamBoundary::operator =(const OpenfoamBoundary& rhs) {
	if (this == &rhs) {
		return *this;
	}
	id_ = rhs.id_;
	name_ = rhs.name_;
	nFaces_ = rhs.nFaces_;
	startFace_ = rhs.startFace_;
	return *this;
}

string
OpenfoamBoundary::getName() const {
	return name_;
}

uint
OpenfoamBoundary::getFaces() const {
	return nFaces_;
}

uint
OpenfoamBoundary::getStartFace() const {
	return startFace_;
}

void
OpenfoamBoundary::printInfo() const {
	cout << " --- OpenFoam boundary info ---" << endl;
	cout << "Name: " << name_ << endl;;
	cout << "Id: " << id_ << endl;
	cout << "Nfaces: " << nFaces_ << endl;
	cout << "StartFace: " << startFace_ << endl;
}

bool
OpenfoamBoundary::isMaterial() const {
	if (name_.find("mat.") != name_.npos) {
		return true;
	}
	return false;
}

bool
OpenfoamBoundary::isOutputRequest() const {
	if (name_.find("out.") != name_.npos) {
		return true;
	}
	return false;
}

uint
OpenfoamBoundary::getMaterialId() const {
	assert(isMaterial());
	string value = name_.substr(name_.find("mat.") + 4, name_.npos);
	return(atoi(value.c_str()));
}

uint
OpenfoamBoundary::getOutputRequestId() const {
	assert(isOutputRequest());
	string value = name_.substr(name_.find("out.") + 4, name_.npos);
	return(atoi(value.c_str()));
}
