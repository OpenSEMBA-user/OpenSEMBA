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
	PMVolumeDispersive();
	virtual ~PMVolumeDispersive();
	PMVolumeDispersive(
	 const uint id_,
	 const string& name_,
	 const double rEps,
	 const double rMu,
	 const double elecCond,
	 const double magnCond);
	PMVolumeDispersive(
	 const uint id_,
	 const string& name_,
	 const double rEps,
	 const double rMu,
	 const double elecCond,
	 const vector<complex<double> >& pole,
	 const vector<complex<double> >& residue,
	 const vector<complex<double> >& drudePole,
	 const vector<complex<double> >& drudeResidue);

	virtual ClassBase* clone() const;

    uint getPoleNumber() const;
    complex<double> getPole(uint p) const;
    complex<double> getResidue(uint p) const;
    uint getDrudePoleNumber() const;
    complex<double> getDrudePole(uint p) const;
    complex<double> getDrudeResidue(uint p) const;
    virtual double getElectricConductivity() const;

    virtual bool isDispersive() const;
    bool isClassic() const;
    bool isSimplyConductive() const;

	void printInfo() const;
protected:
   vector<complex<double> > residue_; // Residues for dispers model. c_p.
   vector<complex<double> > pole_; // Poles for dispersive model. a_p.
   vector<complex<double> > drudeResidue_; // c_p_D
   vector<complex<double> > drudePole_; // Poles for dispersive model. a_p_D.
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
