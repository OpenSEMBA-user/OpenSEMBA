/*
 * PMVolumeDispersive.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolumeDispersive.h"


PMVolumeDispersive::PMVolumeDispersive() {

}

PMVolumeDispersive::~PMVolumeDispersive() {

}

PMVolumeDispersive::PMVolumeDispersive(
 const uint id,
 const string& name,
 const double rEps,
 const double rMu,
 const double elecCond,
 const double magnCond) {
	name_ = name;
	id_ = id;
	rPermittivity = rEps;
	rPermeability = rMu;
	impedance = sqrt((rMu * mu0)/(rEps * eps0));
	admitance = 1 / impedance;
	// Adds conductivity as a permittivity pole.
	if (elecCond != 0.0) {
		pole.push_back(0.0);
		complex<double> elecCondResidual(elecCond/double(2.0)/eps0, 0);
		residue.push_back(elecCondResidual);
	}
	//
	if (magnCond != 0.0) {
		cerr<< "ERROR @ PMVolumeDispersive ctor: "
			<< "Magnetic conductive materials are not supported." << endl;
		printInfo();
	}
	//
	check();
}

PMVolumeDispersive::PMVolumeDispersive(
 const unsigned int id_,
 const string& name_,
 const double rEps,
 const double rMu,
 const double elecCond,
 const vector<complex<double> >& pole_,
 const vector<complex<double> >& residue_,
 const vector<complex<double> >& drudePole_,
 const vector<complex<double> >& drudeResidue_) {
	*this = PMVolumeDispersive(id_, name_, rEps, rMu, elecCond, 0.0);
	pole.insert(pole.end(), pole_.begin(), pole_.end());
	residue.insert(residue.end(), residue_.begin(), residue_.end());
	drudePole.insert(drudePole.end(), drudePole_.begin(), drudePole_.end());
	drudeResidue.insert(drudeResidue.end(), drudeResidue_.begin(), drudeResidue_.end());
	check();
}

bool
PMVolumeDispersive::isSimplyConductive() const {
    return (pole.size() <= 1 && std::abs(getPole(0)) == 0);
}

PMVolumeDispersive&
PMVolumeDispersive::operator=(
 const PMVolumeDispersive &param) {
	if (this == &param) {
		return *this;
	}
	id_ = param.id_;
	name_ = param.name_;
	rPermittivity = param.rPermittivity;
	rPermeability = param.rPermeability;
	impedance = param.impedance;
	admitance = param.admitance;
	residue = param.residue;
	pole = param.pole;
	drudeResidue = param.drudeResidue;
	drudePole = param.drudePole;
	return *this;
}

void
PMVolumeDispersive::printInfo() const {
	cout << "--- PMVolumeDispersive info ---" << endl;
	cout << "Id: " << id_ << endl;
	cout << "Name: " << name_ << endl;
	cout << "Type: " << "Dispersive material" << endl;
	cout << "Rel. permittivity @ inft freq: " << rPermittivity << endl;
	cout << "Rel. permeability @ inft freq: " << rPermeability << endl;
	cout << "Impedance: " << impedance << endl;
	cout << "Admitance: " << admitance << endl;
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

void
PMVolumeDispersive::check() const {
	// Checks validity of parameters.
	if (rPermittivity == 0.0 || rPermeability == 0.0) {
		cerr << "ERROR @ PMVolumeDispersive ctor" << endl;
		cerr << "Invalid values of parameters in material " << id_ << endl;
		exit(PHYSICALMODEL_ERROR);
	}
	if (id_ == 0) {
		cerr << "ERROR @ PMVolumeDispersive ctor" << endl;
		cerr << "Invalid id value." << endl;
		exit(PHYSICALMODEL_ERROR);
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
			return getResidue(i).real() * 2.0 * eps0;
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

