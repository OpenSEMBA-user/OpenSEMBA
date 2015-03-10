/*
 * Waveport.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef WAVEPORT_H_
#define WAVEPORT_H_

#include "EMSource.h"
#include "MagnitudeGaussian.h"
#include "MagnitudeNumerical.h"

class Waveport : public EMSource {
public:
	typedef enum {
		undefined,
		rectangular
	} Shape;
	typedef enum {
		TE,
		TM
	} ExcitationMode;
	Waveport(
	        const vector<ElementId>& elem,
	        const MagnitudeGaussian* magnitude,
	        const bool input,
	        const Shape shape,
	        const ExcitationMode excMode,
	        const pair<UInt,UInt> mode);
	virtual ~Waveport();

	virtual ClassBase* clone() const;

	void printInfo() const;
	Shape getShape() const;
	ExcitationMode getExcitationMode() const;
	pair<UInt, UInt> getMode() const;
private:
	bool input_;
	Shape shape_;
	ExcitationMode excitationMode_;
	pair<UInt,UInt> mode_;
};

#endif /* WAVEPORT_H_ */
