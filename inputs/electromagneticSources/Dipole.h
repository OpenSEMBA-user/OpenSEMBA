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
	 vector<ElementId> elem,
	 Real length,
	 CVecR3 orientation,
	 CVecR3 position,
	 const MagnitudeGaussian* magnitude);
	virtual ~Dipole();

	virtual ClassBase* clone() const;

	void printInfo() const;
protected:
	Real length_;
	CVecR3 orientation_;
	CVecR3 position_;
	Real gaussDelay_;
	Real spreadSqrt2_;
};

#endif /* DIPOLE_H_ */
