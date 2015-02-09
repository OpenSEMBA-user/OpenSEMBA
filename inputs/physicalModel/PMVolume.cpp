/*
 * PMVolume.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolume.h"

PMVolume::PMVolume() {
	rPermittivity = 1.0;
	rPermeability = 1.0;
	admitance = 0.0;
	impedance = 0.0;
	if (rPermittivity != 0.0) {
		impedance = sqrt((rPermeability * mu0) / (rPermittivity * eps0));
		admitance = 1.0 / impedance;
	}
}

PMVolume::~PMVolume() {

}

PMVolume::PMVolume(
 const int id , const string name, const double rEps, const double rMu) {
	id_ = id;
	name_ = name;
	rPermittivity = rEps;
	rPermeability =  rMu;
	if (rEps != 0.0) {
		impedance = sqrt((rMu * mu0) / (rEps * eps0));
		admitance = 1.0 / impedance;
	}
	//
	check();
}

void
PMVolume::check() const {
	if (rPermittivity <= 0.0) {
		cerr<< "ERROR @ PMVolume ctor. : "
		 << "Invalid permittivity value in material " << id_ << endl;
		printInfo();
	}
	if (rPermeability <= 0.0) {
		cerr<< "ERROR @ PMVolume ctor. : "
		<< "Invalid permeability value in material " << id_ << endl;
		printInfo();
	}
}

PMVolume&
PMVolume::operator=(const PMVolume &param) {
	if ( this == &param) {
		return *this;
	}
	id_ = param.id_;
	name_ = param.name_;
	rPermittivity = param.rPermittivity;
	rPermeability = param.rPermeability;
	impedance = param.impedance;
	admitance = param.admitance;
	return *this;
}

void
PMVolume::printInfo() const {
	cout << "--- PMVolume info ---" << endl;
	cout << "Id: " << id_ << endl;
	cout << "Name: " << name_ << endl;
	cout << "Type: " << "Classical material" << endl;
	cout << "Rel. permittivity @ inft freq: " << rPermittivity << endl;
	cout << "Rel. permeability @ inft freq: " << rPermeability << endl;
	cout << "Impedance: " << impedance << endl;
	cout << "Admitance: " << admitance << endl;
}
