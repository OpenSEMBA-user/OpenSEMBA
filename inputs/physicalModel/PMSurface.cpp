/*
 * PMSurface.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMSurface.h"

PMSurface::PMSurface() {

}

PMSurface::~PMSurface() {

}

PMSurface::PMSurface(
 const unsigned int id, const string& name,
 const double Zinfinite_[4], const double Zstatic_[4],
 const vector<double>& pole_,
 const vector<double>& Z11_, const vector<double>& Z12_,
 const vector<double>& Z21_, const vector<double>& Z22_) {
	id_ = id;
	name_ = name;
	for (unsigned int i = 0; i < 4; i++) {
		Zinfinite[i] = Zinfinite_[i];
		Zstatic[i]   = Zstatic_[i];
	}
	pole = pole_;
	Z11 = Z11_;
	Z12 = Z12_;
	Z21 = Z21_;
	Z22 = Z22_;
}

PMSurface&
PMSurface::operator=(
 const PMSurface &param) {
	if (this == &param) {
		return *this;
	}
	// Copies values to object.
	id_ = param.id_;
	name_ = param.name_;
	for (unsigned int j = 0; j < 4; j++) {
		Zinfinite[j] = param.Zinfinite[j];
		Zstatic[j] = param.Zstatic[j];
	}
	pole = param.pole;
	Z11 = param.Z11;
	Z12 = param.Z12;
	Z21 = param.Z21;
	Z22 = param.Z22;
	return *this;
}

void
PMSurface::modifyZinfinite(const double val[4]) {
	for (unsigned int i = 0; i < 4; i++) {
		Zinfinite[i] = val[i];
	}
	return;
}

void
PMSurface::modifyZstatic(const double val[4]) {
	for (unsigned int i = 0; i < 4; i++) {
		Zstatic[i] = val[i];
	}
	return;
}

void
PMSurface::addPole(
 const double vP, const double vZ11, double const vZ12,
 const double vZ21, const double vZ22) {
	pole.push_back(vP);
	Z11.push_back(vZ11);
	Z12.push_back(vZ12);
	Z21.push_back(vZ21);
	Z22.push_back(vZ22);
	return;
}

void
PMSurface::printInfo() const {
	cout << "----- Physical model information -----"   << endl;
	cout << "Id: " << id_ << endl;
	cout << "Name: " << name_ << endl;
	cout << "Type: Surface Impedance Boundary Condition" << endl;
	cout << "Impedance @ infininite freq.:";
	for (unsigned int i = 0; i < 4; i++) {
		cout << " " << Zinfinite[i];
	}
	cout << endl;
	cout << "Impedance @ zero freq.:";
	for (unsigned int i = 0; i < 4; i++) {
		cout << " " << Zstatic[i];
	}
	cout << endl;
	if (pole.size()) {
		cout << "Number of poles: " << pole.size() << endl;
		cout << "# Pole Z11 Z12 Z21 Z22 " << endl;
	}
	for (register unsigned int i = 0; i < pole.size(); i++) {
		cout << i << " "
		     << pole[i] << " "
		     << Z11[i] << " "
		     << Z12[i] << " "
		     << Z21[i] << " "
		     << Z22[i] << endl;
	}
}
