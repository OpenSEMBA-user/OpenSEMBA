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
	 const Magnitude* magnitude);
	virtual ~Dipole();
	Dipole&
	 operator=(const Dipole &rhs);
	void
	 printInfo() const;
private:
	double length_;
	CVecD3 orientation_;
	CVecD3 position_;
};

#endif /* DIPOLE_H_ */
