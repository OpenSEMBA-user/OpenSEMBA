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
		TE,
		TM
	} ExcitationMode;

	Waveport(const Magnitude* magnitude,
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
	CVecR3 getLocalAxis() const;
	CVecR3 getOffset() const;
	virtual vector<CVecR3> toLocalAxis(const vector<CVecR3>& rhs) const;
	virtual vector<CVecR3> toGlobalAxis(const vector<CVecR3>& rhs) const;
	virtual vector<CVecR3> getElectricWeights(
	        const vector<CVecR3>& pos,
	        const BoundTerminations& symmetries) const;


    void printInfo() const;
private:
	ExcitationMode excitationMode_;
	pair<UInt,UInt> mode_;
};

#endif /* WAVEPORT_H_ */
