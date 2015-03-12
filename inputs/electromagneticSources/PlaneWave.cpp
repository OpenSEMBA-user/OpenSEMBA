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
 CVecR3 waveDirection,
 CVecR3 polarization,
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
        cerr << endl << "ERROR @ Planewave: "
            << "Wave direction and polarization cannot be zero" << endl;
        printInfo();
    }
    //
    if ((waveDirection ^ polarization).norm() !=
     waveDirection.norm() * polarization.norm()) {
        cerr << endl << "ERROR @ Planewave: "
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

CVecR3
PlaneWave::getElectricField(const Real time) const {
	CVecR3 res = polarization_ * getMagnitude()->evaluate(time);
	return res;
}

pair<CVecR3, CVecR3>
PlaneWave::getElectromagneticField(const Real time) const {
	CVecR3 electric = getElectricField(time);
	CVecR3 magnetic = (waveDirection_ ^ electric) * (Real) VACUUM_ADMITANCE;
	return pair<CVecR3,CVecR3>(electric, magnetic);
}

const CVecR3&
PlaneWave::getPolarization() const {
	return polarization_;
}

const CVecR3&
PlaneWave::getWaveDirection() const {
	return waveDirection_;
}
