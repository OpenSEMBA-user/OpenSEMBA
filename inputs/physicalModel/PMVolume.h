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
	        const double rEps, const double rMu);
	virtual ~PMVolume();

    virtual ClassBase* clone() const;

    double getRelativePermittivity() const;
    double getPermittivity() const;
    double getRelativePermeability() const;
    double getPermeability() const;
    double getImpedance() const;
    double getAdmitance() const;
    virtual double getElectricConductivity() const;
    virtual double getMagneticConductivity() const;
    bool isVacuum() const;
    virtual bool isVolumic() const;
    virtual bool isClassic() const;
    virtual bool isDispersive() const;
    virtual bool isSimplyConductive() const;
	virtual void printInfo() const;


private:
    double rEps_; // Rel. permittivity @ infte. freq.
    double rMu_; // Rel. permeability @ infte. freq.
};

#endif /* PMVOLUME_H_ */
