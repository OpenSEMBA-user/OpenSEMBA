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
public:
	PMMultiportRLC(
	 const MatId idIn,
	 const string nameIn,
	 const PMMultiport::Type typeIn,
	 const Real resistance,
	 const Real inductance,
	 const Real capacitance);
	virtual ~PMMultiportRLC();

    ClassBase* clone() const;

	virtual Real getR() const;
	virtual Real	 getL() const;
	virtual Real	 getC() const;
	virtual void printInfo() const;
private:
	Real R_, L_, C_;
};

#endif /* PMMULTIPORTRLC_H_ */
