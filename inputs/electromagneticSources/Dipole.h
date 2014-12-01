/*
 * Dipole.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef DIPOLE_H_
#define DIPOLE_H_
#include <math.h>
#include <limits>
#include "EMSource.h"

class Dipole : public EMSource {
public:
	double length;
	CVecD3 orientation;
	CVecD3 position;
	bool sinModulation; // True if sin. modulated.
	double sinAmplitude; // Amplitude of the modulation.
	double frequency; // Frequency of the modulating signal.
	bool gaussModulation; // True if gauss. modulated.
	double gaussAmplitude; // Amplitude of gauss. peak.
	double w; // Angular frequency
	double spreadSqrt2xC0;
	double invSpreadSq;
	Dipole(
	 vector<unsigned int> elem,
	 double length,
	 CVecD3 orientation,
	 CVecD3 position,
	 bool sinModulation,
	 double sinAmplitude,
	 double frequency,
	 bool gaussModulation,
	 double gaussAmplitude,
	 double spread,
	 double delay);
	virtual ~Dipole();
	Dipole&
	 operator=(const Dipole &rhs);
	void
	 printInfo() const;
};

#endif /* DIPOLE_H_ */
