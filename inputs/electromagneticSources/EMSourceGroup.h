/*
 * EMSourceGroup.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef EMSOURCEGROUP_H_
#define EMSOURCEGROUP_H_

#include "EMSource.h"
#include "Dipole.h"
#include "Generator.h"
#include "PlaneWave.h"
#include "Waveport.h"

class EMSourceGroup {
public:
	EMSourceGroup();
	EMSourceGroup(
	 const vector<Dipole>&,
	 const vector<PlaneWave>&,
	 const vector<Waveport>&,
	 const vector<Generator>&);
	EMSourceGroup&
	 operator=(const EMSourceGroup &rhs);
	unsigned int
	 count() const;
	unsigned int
	 countPlaneWaves() const;
	unsigned int
	 countDipoles() const;
	unsigned int
	 countWaveports() const;
	unsigned int
	 countGenerators() const;
	const EMSource*
	 get(const unsigned int i) const;
	const PlaneWave*
	 getPlaneWave() const;
	const PlaneWave*
	 getPlaneWave(const uint i) const;
	const Dipole*
	 getDipole(const unsigned int i) const;
	const Waveport*
	 getWaveport(const unsigned int i) const;
	const Generator*
	 getGenerator(const unsigned int i) const;
	uint
	 countWithType(const Element::Type type) const;
	const EMSource*
	 getWithType(const uint i, const Element::Type type) const;
	void
	 printInfo() const;
	void
	 applyGeometricScalingFactor(const double factor);
private:
	vector<const EMSource*> source;
	vector<Dipole> dipole;
	vector<PlaneWave> planeWave;
	vector<Waveport> waveport;
	vector<Generator> generator;
	void
	 updatePointers();
};

#endif /* EMSOURCEGROUP_H_ */
