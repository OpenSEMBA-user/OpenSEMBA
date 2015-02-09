/*
 * EMSourceGroup.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef EMSOURCEGROUP_H_
	#include "EMSourceGroup.h"
#endif

EMSourceGroup::EMSourceGroup() {

}

EMSourceGroup::EMSourceGroup(
 const vector<Dipole>& dipole_,
 const vector<PlaneWave>& planeWave_,
 const vector<Waveport>& waveport_,
 const vector<Generator>& generator_,
    const vector<Nodal>& nodal_) {
	dipole = dipole_;
	planeWave = planeWave_;
	waveport = waveport_;
	generator = generator_;
   nodal = nodal_;
	updatePointers();
}

EMSourceGroup&
EMSourceGroup::operator=(const EMSourceGroup &rhs) {
	if (this == &rhs) {
		return *this;
	}
	dipole = rhs.dipole;
	planeWave = rhs.planeWave;
	waveport = rhs.waveport;
	generator = rhs.generator;
   nodal = rhs.nodal;
	updatePointers();
	return *this;
}

unsigned int
EMSourceGroup::count() const {
	return source.size();
}

unsigned int
EMSourceGroup::countPlaneWaves() const {
	return planeWave.size();
}

unsigned int
EMSourceGroup::countDipoles() const {
	return dipole.size();
}

unsigned int
EMSourceGroup::countWaveports() const {
	return waveport.size();
}

unsigned int
EMSourceGroup::countGenerators() const {
	return generator.size();
}

unsigned int
EMSourceGroup::countNodals() const {
	return nodal.size();
}

const EMSource*
EMSourceGroup::get(const unsigned int i) const {
	assert(i < count());
	return source[i];
}

const PlaneWave*
EMSourceGroup::getPlaneWave() const {
	assert(countPlaneWaves() == 1);
	return &planeWave[0];
}

const PlaneWave*
EMSourceGroup::getPlaneWave(const uint i) const {
	assert(i < countPlaneWaves());
	return &planeWave[i];
}

const Dipole*
EMSourceGroup::getDipole(const unsigned int i) const {
	assert(i < countDipoles());
	return &dipole[i];
}

const Waveport*
EMSourceGroup::getWaveport(const unsigned int i) const {
	assert (i < countWaveports());
	return &waveport[i];
}

const Generator*
EMSourceGroup::getGenerator(const unsigned int i) const {
	assert (i < countGenerators());
	return &generator[i];
}

const Nodal*
EMSourceGroup::getNodal(const unsigned int i) const {
	assert (i < countNodals());
	return &nodal[i];
}


void
EMSourceGroup::updatePointers() {
	source.clear();
	for (unsigned int i = 0; i < countDipoles(); i++) {
		source.push_back(getDipole(i));
	}
	for (unsigned int i = 0; i < countPlaneWaves(); i++) {
		source.push_back(getPlaneWave());
	}
	for (unsigned int i = 0; i < countWaveports(); i++) {
		source.push_back(getWaveport(i));
	}
	for (unsigned int i = 0; i < countGenerators(); i++) {
		source.push_back(getGenerator(i));
	}
   for (unsigned int i = 0; i < countNodals(); i++) {
		source.push_back(getNodal(i));
	}
}

void
EMSourceGroup::printInfo() const {
	cout << " --- EMSourceGroup info --- " << endl;
	for (unsigned int i = 0; i < count(); i++) {
		source[i]->printInfo();
	}
}

uint
EMSourceGroup::countWithType(const Element::Type type) const {
	uint res = 0;
	for (uint i = 0; i < source.size(); i++) {
		if (source[i]->getElementType() == type) {
			res++;
		}
	}
	return res;
}

const EMSource*
EMSourceGroup::getWithType(
 const uint j,
 const Element::Type param) const {
	assert(j < countWithType(param));
	uint count = 0;
	for (uint i = 0; i < source.size(); i++) {
		if (source[i]->getElementType() == param) {
			if (count == j) {
				return source[i];
			} else {
				count++;
			}
		}
	}
	cerr<< "ERROR @ EMSourceGroup: "
		<< "EMSource was not found in group." << endl;
	assert(false);
	return NULL;
}

void
EMSourceGroup::applyGeometricScalingFactor(const double factor) {
    for (uint i = 0; i < countPlaneWaves(); i++) {
        planeWave[i].applyGeometricScalingFactor(factor);
    }
}
