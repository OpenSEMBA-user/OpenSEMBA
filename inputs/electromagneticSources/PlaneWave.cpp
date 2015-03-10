/*
 * PlaneWave.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "PlaneWave.h"

PlaneWave::PlaneWave() {

}

PlaneWave::PlaneWave(
 vector<ElementId> elem,
 CVecD3 waveDirection,
 CVecD3 polarization,
 const Magnitude* magnitude) : EMSource(elem, magnitude) {
    waveDirection_ = waveDirection;
    polarization_ = polarization;
    if (polarization_.norm() == 0) {
        cout<< "ERROR @ PlaneWave: " << "Polarization can't be zero." << endl;
        printInfo();
    }
    if (waveDirection_.norm() == 0) {
        cout<< "ERROR @ PlaneWave: " << "W. Direction can't be zero." << endl;
        printInfo();
    }
    if (waveDirection_.norm() == 0.0 || polarization.norm() == 0.0) {
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
}

PlaneWave::~PlaneWave() {

}

ClassBase* PlaneWave::clone() const {
    return new PlaneWave(*this);
}

void
PlaneWave::printInfo() const {
	cout<< " --- PlaneWave info --- " << endl;
	EMSource::printInfo();
	cout<< " - Polarization vector: " << polarization_ << endl;
	cout<< " - Wave direction vector: " << waveDirection_ << endl;
}

CVecD3
PlaneWave::getElectricField(const double time) const {
	CVecD3 res = polarization_ * getMagnitude()->evaluate(time);
	return res;
}

pair<CVecD3, CVecD3>
PlaneWave::getElectromagneticField(const double time) const {
	CVecD3 electric = getElectricField(time);
	CVecD3 magnetic = (waveDirection_ ^ electric) * (double) VACUUM_ADMITANCE;
	return pair<CVecD3,CVecD3>(electric, magnetic);
}

const CVecD3&
PlaneWave::getPolarization() const {
	return polarization_;
}

const CVecD3&
PlaneWave::getWaveDirection() const {
	return waveDirection_;
}
