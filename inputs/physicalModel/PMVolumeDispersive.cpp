/*
 * PMVolumeDispersive.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolumeDispersive.h"

PMVolumeDispersive::~PMVolumeDispersive() {

}

PMVolumeDispersive::PMVolumeDispersive(
 const uint id,
 const string& name,
 const double rEps,
 const double rMu,
 const double elecCond,
 const double magnCond) : PMVolume(id, name, rEps, rMu) {
	// Adds conductivity as a permittivity pole.
	if (elecCond != 0.0) {
		pole.push_back(0.0);
		complex<double> elecCondResidual(elecCond/double(2.0)/Constants::eps0, 0);
		residue.push_back(elecCondResidual);
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
 const vector<complex<double> >& pole_,
 const vector<complex<double> >& residue_,
 const vector<complex<double> >& drudePole_,
 const vector<complex<double> >& drudeResidue_)
  : PMVolume(id, name, rEps, rMu) {
	*this = PMVolumeDispersive(id, name, rEps, rMu, elecCond, 0.0);
	pole.insert(pole.end(), pole_.begin(), pole_.end());
	residue.insert(residue.end(), residue_.begin(), residue_.end());
	drudePole.insert(drudePole.end(), drudePole_.begin(), drudePole_.end());
	drudeResidue.insert(drudeResidue.end(), drudeResidue_.begin(), drudeResidue_.end());
}

bool
PMVolumeDispersive::isSimplyConductive() const {
    return (pole.size() <= 1 && std::abs(getPole(0)) == 0);
}

void
PMVolumeDispersive::printInfo() const {
	cout << "--- PMVolumeDispersive info ---" << endl;
	PMVolume::printInfo();
	cout << "Type: " << "Dispersive material" << endl;
	cout << "Number of pole residue pairs: " << pole.size() << endl;
	cout << "# " << " re_a " << " im_a " << " re_c " << " im_c " << endl;
	for (unsigned int i = 0; i < pole.size(); i++) {
		cout << i << " " << pole[i].real() << " " << pole[i].imag()
		 << " " << residue[i].real()
		 << " " << residue[i].imag() << endl;
	}
	cout << "Number of Drude Pole Residue pairs: " <<
	 getDrudePoleNumber() << endl;
	cout << "# " << " re_a " << " im_a " << " re_c " << " im_c " << endl;
	for (unsigned int i = 0; i < getDrudePoleNumber(); i++) {
		cout << i << " " << drudePole[i].real() << " " << drudePole[i].imag()
	     << " " << drudeResidue[i].real()
	     << " " << drudeResidue[i].imag() << endl;
	}
}

double
PMVolumeDispersive::getElectricConductivity() const {
	if (pole.size() > 1) {
		cerr<< "WARNING @ getElectricConductivity: "
			<< "This material is dispersive and its effective permittivity "
			<< "depends on several parameters."
			<< "Returning static limit conductivity." << endl;
	}
	for (uint i = 0; i < pole.size(); i++) {
		if (std::abs(getPole(i)) == 0) {
			return getResidue(i).real() * 2.0 * Constants::eps0;
		}
	}
	return 0.0;
}

void
PMVolumeDispersive::addDrudePole(
 const complex<double>& pole_, const complex<double>& res_) {
	drudePole.push_back(pole_);
	drudeResidue.push_back(res_);
	return;
}

void
PMVolumeDispersive::addPole(
 const complex<double>& pole_, const complex<double>& res_) {
	pole.push_back(pole_);
	residue.push_back(res_);
	return;
}

