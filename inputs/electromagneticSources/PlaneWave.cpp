/*
 * PlaneWave.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef PLANEWAVE_H_
	#include "PlaneWave.h"
#endif

PlaneWave::PlaneWave(
 vector<unsigned int> elem_,
 CVecD3 waveDirection_,
 CVecD3 polarization_,
 double spread_,
 double delay_,
 string filename_) {
	elem = elem_;
	usingBound = false;
	init(waveDirection_, polarization_, spread_, delay_, filename_);
}

PlaneWave::PlaneWave(
 pair<CVecD3,CVecD3> bound_,
 CVecD3 waveDirection_,
 CVecD3 polarization_,
 double spread_,
 double delay_,
 string filename_) {
	usingBound = true;
	bound = bound_;
	init(waveDirection_, polarization_, spread_, delay_, filename_);
}

PlaneWave::~PlaneWave() {

}

void
PlaneWave::init(
 const CVecD3& waveDirection_, const CVecD3& polarization_,
 double spread_, double delay_, const string& filename_) {
	waveDirection = waveDirection_;
	polarization = polarization_;
	spread = spread_;
	delay = delay_;
	filename = filename_;
	if (polarization.norm() == 0) {
		printInfo();
		cout<< "ERROR@PlaneWave" << endl;
		cout<< "Polarization can't be zero." << endl;
		exit(-1);
	}
	if (waveDirection.norm() == 0) {
		printInfo();
		cout<< "ERROR@PlaneWave" << endl;
		cout<< "Wave direction can't be zero." << endl;
		exit(-1);
	}
	if (waveDirection.norm() == 0.0 || polarization.norm() == 0.0) {
		cerr<< "ERROR @ Planewave: "
			<< "Wave direction and polarization cannot be zero" << endl;
		printInfo();
	}
	//
	if ((waveDirection ^ polarization).norm() !=
	 waveDirection.norm() * polarization.norm()) {
		cerr<< "ERROR @ Planewave: "
		    << "Wavedirection is not perpendicular to polarization." << endl;
	}
	spreadSqrt2 = getSpread() * sqrt(2.0);
}

PlaneWave&
PlaneWave::operator=(const PlaneWave &rhs) {
	if (this == &rhs) {
		return *this;
	}
	elem = rhs.elem;
	waveDirection = rhs.waveDirection;
	polarization = rhs.polarization;
	spread = rhs.spread;
	delay = rhs.delay;
	filename = rhs.filename;
	usingBound = rhs.usingBound;
	bound = rhs.bound;
	return *this;
}

BoundingBox
PlaneWave::getBound() const {
	if (usingBound) {
		return bound;
	} else {
		cerr << "ERROR @ Planewave" << endl;
		cerr << "Not using bound." << endl;
		exit(-1);
	}
}

void
PlaneWave::printInfo() const {
	cout<< " --- PlaneWave info --- " << endl;
	cout<< " - Assigned on " << elem.size() << " elements." << endl;
	cout<< " - Polarization vector: ";
	polarization.printInfo();
	cout<< endl;
	cout<< " - Wave direction vector: ";
	waveDirection.printInfo();
	cout<< endl;
	cout<< " - Plane wave exc modulated with a gaussian." << endl;
	cout<< " - Gaussian spread: " << spread << endl;
	cout<< " - Gaussian delay:	" << delay << endl;
	cout<< " - Excitation filename: " << filename << endl;
	if (usingBound) {
		bound.printInfo();
	}
}

CVecD3
PlaneWave::getElectricField(const double time) const {
	double expArg = (time - delay) / (spreadSqrt2);
	double amp = exp(-expArg * expArg);
	CVecD3 res = polarization * amp;
	return res;
}

pair<CVecD3, CVecD3>
PlaneWave::getElectromagneticField(const double time) const {
	CVecD3 electric = getElectricField(time);
	CVecD3 magnetic = (waveDirection ^ electric) * (double) VACUUM_ADMITANCE;
	return pair<CVecD3,CVecD3>(electric, magnetic);
}

const CVecD3&
PlaneWave::getPolarization() const {
	return polarization;
}

const CVecD3&
PlaneWave::getWaveDirection() const {
	return waveDirection;
}
