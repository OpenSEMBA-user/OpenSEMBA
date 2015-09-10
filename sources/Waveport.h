/*
 * Waveport.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef WAVEPORT_H_
#define WAVEPORT_H_

#include "../sources/EMSource.h"
#include "../options/OptionsMesher.h"
#include "base/error/ErrorNotImplemented.h"


class Waveport : public EMSource<Surf> {
public:
	typedef enum {
		TE,
		TM
	} ExcitationMode;

	Waveport(const Magnitude& magnitude,
             const GroupElements<const Surf>& elem,
	         const ExcitationMode excMode,
	         const pair<UInt,UInt> mode);
	Waveport(const Waveport& rhs);
	virtual ~Waveport();

    bool hasSameProperties(const EMSourceBase& rhs) const;

    const string& getName() const;
	ExcitationMode getExcitationMode() const;
	pair<UInt, UInt> getMode() const;

	CVecR3 getNormal() const;

	virtual CVecR3 getOrigin(const BoundTerminations& sym) const = 0;
	virtual CVecR3 getWeight(
	        const CVecR3& pos,
	        const BoundTerminations& sym) const = 0;


    void printInfo() const;
private:
	ExcitationMode excitationMode_;
	pair<UInt,UInt> mode_;
};

#endif /* WAVEPORT_H_ */
