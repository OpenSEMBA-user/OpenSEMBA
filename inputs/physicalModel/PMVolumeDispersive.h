/*
 * PMVolumeDispersive.h
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#ifndef PMVOLUMEDISPERSIVE_H_
#define PMVOLUMEDISPERSIVE_H_

#include "PMVolume.h"

class PMVolumeDispersive : public PMVolume {
public:
	vector<complex<double> > residue; // Residues for dispers model. c_p.
	vector<complex<double> > pole; // Poles for dispersive model. a_p.
	vector<complex<double> > drudeResidue; // c_p_D
	vector<complex<double> > drudePole; // Poles for dispersive model. a_p.
	PMVolumeDispersive();
	virtual ~PMVolumeDispersive();
	PMVolumeDispersive(
	 const unsigned int id_,
	 const string& name_,
	 const double rEps,
	 const double rMu,
	 const double elecCond,
	 const double magnCond);
	PMVolumeDispersive(
	 const unsigned int id_,
	 const string& name_,
	 const double rEps,
	 const double rMu,
	 const double elecCond,
	 const vector<complex<double> >& pole_,
	 const vector<complex<double> >& residue_,
	 const vector<complex<double> >& drudePole_,
	 const vector<complex<double> >& drudeResidue_);
	virtual PMVolumeDispersive&
	 operator=(const PMVolumeDispersive &param);
//	virtual PMVolumeDispersive&
//	 operator=(const PhysicalModel &param);
	long unsigned int
	 getPoleNumber() const {return pole.size();}
	complex<double>
	 getPole(unsigned int p) const {return pole[p];}
	complex<double>
	 getResidue(unsigned int p) const {return  residue[p];}
	unsigned int
	 getDrudePoleNumber() const {return drudePole.size();}
	complex<double>
	 getDrudePole(unsigned int p) const {return drudePole[p];}
	complex<double>
	 getDrudeResidue(unsigned int p) const {return drudeResidue[p];}
	virtual bool
	 isDispersive() const {
		return true;
	 }
	double
	 getElectricConductivity() const;
	void
	 check() const;
	void
	 printInfo() const;
private:
	void
	 addPole(
	  const complex<double>& pole_,
	  const complex<double>& res_);
	void
	 addDrudePole(
	  const complex<double>& pole_,
	  const complex<double>& res_);
};

#endif /* PMVOLUMEDISPERSIVE_H_ */
