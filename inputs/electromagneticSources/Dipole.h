/*
 * Dipole.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef DIPOLE_H_
#define DIPOLE_H_
#include "EMSource.h"

class Dipole : public EMSource {
public:
	Dipole(
	 vector<unsigned int> elem,
	 double length,
	 CVecD3 orientation,
	 CVecD3 position,
	 const MagnitudeGaussian* magnitude);
	virtual ~Dipole();

	virtual ClassBase* clone() const;

	void printInfo() const;
protected:
	double length_;
	CVecD3 orientation_;
	CVecD3 position_;
	double gaussDelay_;
	double spreadSqrt2_;
};

#endif /* DIPOLE_H_ */
