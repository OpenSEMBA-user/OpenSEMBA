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
	PlaneWave&
	 operator=(const PlaneWave &rhs);
	bool
	 isPlaneWave() const {return true;}
	void
	 printInfo() const;
	const BoxD3*
	 getBound() const;
	const CVecD3&
	 getPolarization() const;
	const CVecD3&
	 getWaveDirection() const;
	CVecD3
	 getElectricField(const double time) const;
	pair<CVecD3,CVecD3>
	 getElectromagneticField(const double time) const;
    void
     applyGeometricScalingFactor(const double factor);
private:
	CVecD3 waveDirection_;
	CVecD3 polarization_;
	BoxD3* bound_;
	void
	init(
	 const CVecD3& waveDirection,
 	 const CVecD3& polarization);
};

#endif /* PLANEWAVE_H_ */
