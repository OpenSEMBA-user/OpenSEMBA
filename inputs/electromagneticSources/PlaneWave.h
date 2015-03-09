/*
 * PlaneWave.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef PLANEWAVE_H_
#define PLANEWAVE_H_

#include "EMSource.h"

class PlaneWave : public EMSource {
public:
    PlaneWave();
    PlaneWave(
	 vector<unsigned int> elem,
	 CVecD3 waveDirection,
	 CVecD3 polarization,
	 const Magnitude* magnitude);
	PlaneWave(
	 BoxD3 bound,
	 CVecD3 waveDirection,
	 CVecD3 polarization,
	 const Magnitude* magnitude);
	virtual ~PlaneWave();

	virtual ClassBase* clone() const;

	PlaneWave& operator=(const PlaneWave &rhs);

	const CVecD3& getPolarization() const;
	const CVecD3& getWaveDirection() const;
	CVecD3 getElectricField(const double time) const;
	pair<CVecD3,CVecD3> getElectromagneticField(const double time) const;
	void printInfo() const;
private:
	CVecD3 waveDirection_;
	CVecD3 polarization_;
	void
	init(
	 const CVecD3& waveDirection,
 	 const CVecD3& polarization);
};

#endif /* PLANEWAVE_H_ */
