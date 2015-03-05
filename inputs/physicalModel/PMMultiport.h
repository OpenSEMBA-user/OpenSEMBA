/*
 * PMMultiport.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMMULTIPORT_H_
#define PMMULTIPORT_H_

#include "PhysicalModel.h"

class PMMultiport: public PhysicalModel {
public:
	typedef enum {
		// Predefined
		shortCircuit = 1,
		openCircuit = 2,
		matched = 3,
		// RLC types.
		sRLC = 11,    // R+L+C.
		sRLpC = 12,   // (R + L)//C.
		sRCpL = 13,   // (R + C)//L.
		sRpLC = 14,   // R + (L//C).
		sLCpR = 15,   // (L + C)//R.
		sLpRC = 16,   // L + (R//C).
		sCpLR = 17,   // C + (L//R).
		pRLC = 18,     // R//L//C.
		undefined = 0
 	} Type;
 	PMMultiport(const uint id, const string name);
	virtual ~PMMultiport();
	virtual bool isMultiport() const {return true;}
	virtual Type getType() const;
protected:
	Type type_;
	string getTypeStr() const;
};

#endif /* PMMULTIPORT_H_ */
