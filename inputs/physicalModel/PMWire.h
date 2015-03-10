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
	 const MatId id,
	 const string name,
	 const Real radius,
	 const Real resistance,
	 const Real inductance);
	virtual ~PMWire();

    ClassBase* clone() const;

	virtual Real	 getRadius() const;
	virtual Real	 getResistance() const;
	virtual Real	 getInductance() const;
	virtual void printInfo() const;
private:
	Real radius_;
	Real resistance_; // Resistance per meter.
	Real inductance_; // Inductance per meter.
};

#endif /* PMWIRE_H_ */
