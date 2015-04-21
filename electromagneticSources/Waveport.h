/*
 * Waveport.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef WAVEPORT_H_
#define WAVEPORT_H_

#include "EMSource.h"

class Waveport : public EMSource<Surf> {
public:
	typedef enum {
		undefined,
		rectangular
	} Shape;
	typedef enum {
		TE,
		TM
	} ExcitationMode;

	Waveport(const MagnitudeGaussian* magnitude,
             const ElementsGroup<Surf>& elem,
	         const bool input,
	         const Shape shape,
	         const ExcitationMode excMode,
	         const pair<UInt,UInt> mode);
	Waveport(const Waveport& rhs);
	virtual ~Waveport();

    DEFINE_CLONE(Waveport);

    bool hasSameProperties(const EMSourceBase& rhs) const;

    const string& getName() const;
	Shape getShape() const;
	ExcitationMode getExcitationMode() const;
	pair<UInt, UInt> getMode() const;

    void printInfo() const;
private:
	bool input_;
	Shape shape_;
	ExcitationMode excitationMode_;
	pair<UInt,UInt> mode_;
};

#endif /* WAVEPORT_H_ */
