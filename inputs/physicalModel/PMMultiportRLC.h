/*
 * PMMultiportRLC.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMMULTIPORTRLC_H_
#define PMMULTIPORTRLC_H_

#include "PMMultiport.h"

class PMMultiportRLC: public PMMultiport {
	// From Amelet HDF 1.5.3.
public:
	PMMultiportRLC();
	PMMultiportRLC(
	 const uint idIn,
	 const string nameIn,
	 const PMMultiport::Type typeIn,
	 const double resistance,
	 const double inductance,
	 const double capacitance);
	virtual ~PMMultiportRLC();
	virtual double
	 getR() const;
	virtual double
	 getL() const;
	virtual double
	 getC() const;
	virtual bool
	 isMultiportRLC() const {return true;}
	virtual void
	 printInfo() const;
private:
	double R_, L_, C_;
};

#endif /* PMMULTIPORTRLC_H_ */
