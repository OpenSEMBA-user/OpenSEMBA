/*
 * PMVolume.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMVOLUME_H_
#define PMVOLUME_H_

#include "PhysicalModel.h"

class PMVolume : public PhysicalModel {
public:
	PMVolume(
	        const MatId id, const string name,
	        const Real rEps, const Real rMu);
	virtual ~PMVolume();

	DEFINE_CLONE(PMVolume);

    Real getRelativePermittivity() const;
    Real getPermittivity() const;
    Real getRelativePermeability() const;
    Real getPermeability() const;
    Real getImpedance() const;
    Real getAdmitance() const;
    virtual Real getElectricConductivity() const;
    virtual Real getMagneticConductivity() const;
    bool isVacuum() const;
    virtual bool isVolumic() const;
    virtual bool isClassic() const;
    virtual bool isDispersive() const;
    virtual bool isSimplyConductive() const;
	virtual void printInfo() const;


private:
    Real rEps_; // Rel. permittivity @ infte. freq.
    Real rMu_; // Rel. permeability @ infte. freq.
};

#endif /* PMVOLUME_H_ */
