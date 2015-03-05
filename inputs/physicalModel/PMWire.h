/*
 * PMWire.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMWIRE_H_
#define PMWIRE_H_

#include "PhysicalModel.h"

class PMWire: public PhysicalModel {
public:
	PMWire(
	 const uint id,
	 const string name,
	 const double radius,
	 const double resistance,
	 const double inductance);
	virtual ~PMWire();
	virtual double
	 getRadius() const;
	virtual double
	 getResistance() const;
	virtual double
	 getInductance() const;
	virtual bool
	 isWire() const {return true;}
	virtual void
	 printInfo() const;
private:
	double radius_;
	double resistance_; // Resistance per meter.
	double inductance_; // Inductance per meter.
};

#endif /* PMWIRE_H_ */
