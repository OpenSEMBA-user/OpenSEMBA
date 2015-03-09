/*
 * PMVolumeDispersive.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolumeDispersive.h"

PMVolumeDispersive::PMVolumeDispersive(
 const uint id,
 const string& name,
 const double rEps,
 const double rMu,
 const double elecCond,
 const double magnCond) : PMVolume(id, name, rEps, rMu) {
	// Adds conductivity as a permittivity pole.
	if (elecCond != 0.0) {
		pole_.push_back(0.0);
		complex<double> elecCondResidual(elecCond/double(2.0)/Constants::eps0, 0);
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
 const unsigned int id,
 const string& name,
 const double rEps,
 const double rMu,
 const double elecCond,
 const vector<complex<double> >& pole,
 const vector<complex<double> >& residue,
 const vector<complex<double> >& drudePole,
 const vector<complex<double> >& drudeResidue)
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

uint PMVolumeDispersive::getDrudePoleNumber() const {
    return drudePole_.size();
}


inline uint PMVolumeDispersive::getPoleNumber() const {
    return pole_.size();
}

inline complex<double> PMVolumeDispersive::getPole(uint p) const {
    return pole_[p];
}

inline complex<double> PMVolumeDispersive::getResidue(uint p) const {
    return residue_[p];
}

inline complex<double> PMVolumeDispersive::getDrudePole(uint p) const {
    return drudePole_[p];
}

inline complex<double> PMVolumeDispersive::getDrudeResidue(uint p) const {
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
	for (unsigned int i = 0; i < pole_.size(); i++) {
		cout << i << " " << pole_[i].real() << " " << pole_[i].imag()
		 << " " << residue_[i].real()
		 << " " << residue_[i].imag() << endl;
	}
	cout << "Number of Drude Pole Residue pairs: " <<
	 getDrudePoleNumber() << endl;
	cout << "# " << " re_a " << " im_a " << " re_c " << " im_c " << endl;
	for (unsigned int i = 0; i < getDrudePoleNumber(); i++) {
		cout << i << " " << drudePole_[i].real() << " " << drudePole_[i].imag()
	     << " " << drudeResidue_[i].real()
	     << " " << drudeResidue_[i].imag() << endl;
	}
}

double
PMVolumeDispersive::getElectricConductivity() const {
	if (pole_.size() > 1) {
		cerr<< "WARNING @ getElectricConductivity: "
			<< "This material is dispersive and its effective permittivity "
			<< "depends on several parameters."
			<< "Returning static limit conductivity." << endl;
	}
	for (uint i = 0; i < pole_.size(); i++) {
		if (std::abs(getPole(i)) == 0) {
			return getResidue(i).real() * 2.0 * Constants::eps0;
		}
	}
	return 0.0;
}

void
PMVolumeDispersive::addDrudePole(
 const complex<double>& pole, const complex<double>& res_) {
	drudePole_.push_back(pole);
	drudeResidue_.push_back(res_);
	return;
}

void
PMVolumeDispersive::addPole(
 const complex<double>& pole, const complex<double>& res_) {
	pole_.push_back(pole);
	residue_.push_back(res_);
	return;
}

