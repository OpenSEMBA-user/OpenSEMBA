/*
 * PMVolumeDispersive.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolumeDispersive.h"

PMVolumeDispersive::PMVolumeDispersive(
 const MatId id,
 const string& name,
 const Real rEps,
 const Real rMu,
 const Real elecCond,
 const Real magnCond) : PMVolume(id, name, rEps, rMu) {
	// Adds conductivity as a permittivity pole.
	if (elecCond != 0.0) {
		pole_.push_back(0.0);
		complex<Real> elecCondResidual(elecCond/Real(2.0)/Constants::eps0, 0);
		residue_.push_back(elecCondResidual);
	}
	//
	if (magnCond != 0.0) {
		cerr<< "ERROR @ PMVolumeDispersive ctor: "
			<< "Magnetic conductive materials are not supported." << endl;
		printInfo();
	}
}

PMVolumeDispersive::PMVolumeDispersive(
 const MatId id,
 const string& name,
 const Real rEps,
 const Real rMu,
 const Real elecCond,
 const vector<complex<Real> >& pole,
 const vector<complex<Real> >& residue,
 const vector<complex<Real> >& drudePole,
 const vector<complex<Real> >& drudeResidue)
  : PMVolume(id, name, rEps, rMu) {
	*this = PMVolumeDispersive(id, name, rEps, rMu, elecCond, 0.0);
	pole_.insert(pole_.end(), pole.begin(), pole.end());
	residue_.insert(residue_.end(), residue.begin(), residue.end());
	drudePole_.insert(drudePole_.end(), drudePole.begin(), drudePole.end());
	drudeResidue_.insert(drudeResidue_.end(), drudeResidue.begin(), drudeResidue.end());
}

PMVolumeDispersive::~PMVolumeDispersive() {

}

ClassBase* PMVolumeDispersive::clone() const {
   return new PMVolumeDispersive(*this);
}

UInt PMVolumeDispersive::getDrudePoleNumber() const {
    return drudePole_.size();
}


inline UInt PMVolumeDispersive::getPoleNumber() const {
    return pole_.size();
}

inline complex<Real> PMVolumeDispersive::getPole(UInt p) const {
    return pole_[p];
}

inline complex<Real> PMVolumeDispersive::getResidue(UInt p) const {
    return residue_[p];
}

inline complex<Real> PMVolumeDispersive::getDrudePole(UInt p) const {
    return drudePole_[p];
}

inline complex<Real> PMVolumeDispersive::getDrudeResidue(UInt p) const {
    return drudeResidue_[p];
}

inline bool PMVolumeDispersive::isDispersive() const {
    return true;
}

inline bool PMVolumeDispersive::isClassic() const {
    return isSimplyConductive();
}


bool
PMVolumeDispersive::isSimplyConductive() const {
    return (pole_.size() <= 1 && std::abs(getPole(0)) == 0);
}

void
PMVolumeDispersive::printInfo() const {
	cout << "--- PMVolumeDispersive info ---" << endl;
	PMVolume::printInfo();
	cout << "Type: " << "Dispersive material" << endl;
	cout << "Number of pole residue pairs: " << pole_.size() << endl;
	cout << "# " << " re_a " << " im_a " << " re_c " << " im_c " << endl;
	for (UInt i = 0; i < pole_.size(); i++) {
		cout << i << " " << pole_[i].real() << " " << pole_[i].imag()
		 << " " << residue_[i].real()
		 << " " << residue_[i].imag() << endl;
	}
	cout << "Number of Drude Pole Residue pairs: " <<
	 getDrudePoleNumber() << endl;
	cout << "# " << " re_a " << " im_a " << " re_c " << " im_c " << endl;
	for (UInt i = 0; i < getDrudePoleNumber(); i++) {
		cout << i << " " << drudePole_[i].real() << " " << drudePole_[i].imag()
	     << " " << drudeResidue_[i].real()
	     << " " << drudeResidue_[i].imag() << endl;
	}
}

Real
PMVolumeDispersive::getElectricConductivity() const {
	if (pole_.size() > 1) {
		cerr<< "WARNING @ getElectricConductivity: "
			<< "This material is dispersive and its effective permittivity "
			<< "depends on several parameters."
			<< "Returning static limit conductivity." << endl;
	}
	for (UInt i = 0; i < pole_.size(); i++) {
		if (std::abs(getPole(i)) == 0) {
			return getResidue(i).real() * 2.0 * Constants::eps0;
		}
	}
	return 0.0;
}

void
PMVolumeDispersive::addDrudePole(
 const complex<Real>& pole, const complex<Real>& res_) {
	drudePole_.push_back(pole);
	drudeResidue_.push_back(res_);
	return;
}

void
PMVolumeDispersive::addPole(
 const complex<Real>& pole, const complex<Real>& res_) {
	pole_.push_back(pole);
	residue_.push_back(res_);
	return;
}

