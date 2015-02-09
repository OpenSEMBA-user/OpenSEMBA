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
	double rPermittivity; // Rel. permittivity @ infte. freq.
	double rPermeability; // Rel. permeability @ infte. freq.
	double impedance; // Impedance @ infte. freq.
	double admitance; // Admitance @ infte. freq.
	PMVolume();
	PMVolume(
     const int id_, const string name_,
     const double rEps, const double rMu);
	virtual ~PMVolume();
	virtual PMVolume&
	 operator=(const PMVolume &param);
	double
	 getRelativePermittivity() const {return rPermittivity;}
	double
	 getRelativePermeability() const {return rPermeability;}
	double
	 getImpedance() const {return impedance;}
	double
	 getAdmitance() const {return admitance;}
	virtual double
	 getElectricConductivity() const {return (double) 0.0;}
	virtual double
	 getMagneticConductivity() const {return (double) 0.0;}
	bool
	 isVacuum() const {
	 	return (rPermittivity==1.0 && rPermeability==1.0);
	 }
	virtual bool
	 isVolumic() const {return true;}
	virtual bool
	 isClassic() const {
	    return true;
	}
	virtual bool
	 isDispersive() const {
		return false;
	 }
	virtual bool
	 isSimplyConductive() const {
	    return false;
	}
	virtual void
	 printInfo() const;
protected:
private:
	virtual void
	 check() const;
};


#endif /* PMVOLUME_H_ */
