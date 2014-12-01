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
	PlaneWave(
	 vector<unsigned int> elem,
	 CVecD3 waveDirection,
	 CVecD3 polarization,
	 double spread,
	 double delay,
	 string filename_);
	PlaneWave(
	 pair<CVecD3,CVecD3> bound,
	 CVecD3 waveDirection,
	 CVecD3 polarization,
	 double spread,
	 double delay,
	 string filename);
	virtual ~PlaneWave();
	PlaneWave&
	 operator=(const PlaneWave &rhs);
	bool
	 isPlaneWave() const {return true;}
	void
	 printInfo() const;
	BoundingBox
	 getBound() const;
	const CVecD3&
	 getPolarization() const;
	const CVecD3&
	 getWaveDirection() const;
	CVecD3
	 getElectricField(const double time) const;
	pair<CVecD3,CVecD3>
	 getElectromagneticField(const double time) const;
private:
	CVecD3 waveDirection;
	CVecD3 polarization;
	bool usingBound;
	BoundingBox bound;
	double spreadSqrt2;
	void
	init(
	 const CVecD3& waveDirection_,
 	 const CVecD3& polarization_,
 	 double spread_,
	 double delay_,
	 const string& filename_);
};

#endif /* PLANEWAVE_H_ */
