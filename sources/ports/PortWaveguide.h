/*
 * Waveport.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef WAVEPORT_H_
#define WAVEPORT_H_

#include "Port.h"

class PortWaveguide : public Port {
public:
	typedef enum {
		TE,
		TM
	} ExcitationMode;

	PortWaveguide(Magnitude* magnitude,
             const GroupElements<const Surf>& elem,
	         const ExcitationMode excMode,
	         const pair<UInt,UInt> mode);
	PortWaveguide(const PortWaveguide& rhs);
	virtual ~PortWaveguide();

    bool hasSameProperties(const EMSourceBase& rhs) const;

	ExcitationMode getExcitationMode() const;
	pair<UInt, UInt> getMode() const;

    void printInfo() const;
private:
	ExcitationMode excitationMode_;
	pair<UInt,UInt> mode_;
};

#endif /* WAVEPORT_H_ */
