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
	 const MatId id_,
	 const string& name_,
	 const Real rEps,
	 const Real rMu,
	 const Real elecCond,
	 const Real magnCond);
	PMVolumeDispersive(
	 const MatId id_,
	 const string& name_,
	 const Real rEps,
	 const Real rMu,
	 const Real elecCond,
	 const vector<complex<Real> >& pole,
	 const vector<complex<Real> >& residue,
	 const vector<complex<Real> >& drudePole,
	 const vector<complex<Real> >& drudeResidue);

    DEFINE_CLONE(PMVolumeDispersive);

    UInt getPoleNumber() const;
    complex<Real> getPole(UInt p) const;
    complex<Real> getResidue(UInt p) const;
    UInt getDrudePoleNumber() const;
    complex<Real> getDrudePole(UInt p) const;
    complex<Real> getDrudeResidue(UInt p) const;
    virtual Real getElectricConductivity() const;

    virtual bool isDispersive() const;
    bool isClassic() const;
    bool isSimplyConductive() const;

	void printInfo() const;
protected:
   vector<complex<Real> > residue_; // Residues for dispers model. c_p.
   vector<complex<Real> > pole_; // Poles for dispersive model. a_p.
   vector<complex<Real> > drudeResidue_; // c_p_D
   vector<complex<Real> > drudePole_; // Poles for dispersive model. a_p_D.
	void
	 addPole(
	  const complex<Real>& pole_,
	  const complex<Real>& res_);
	void
	 addDrudePole(
	  const complex<Real>& pole_,
	  const complex<Real>& res_);
};

#endif /* PMVOLUMEDISPERSIVE_H_ */
