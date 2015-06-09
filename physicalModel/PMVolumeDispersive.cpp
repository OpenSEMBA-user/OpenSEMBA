/*
 * PMVolumeDispersive.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMVolumeDispersive.h"

PMVolumeDispersive::ErrorMagneticMaterial::ErrorMagneticMaterial()
:   Error("PMVolumeDispersive: "
          "Magnetic conductive materials are not supported.") {

}

PMVolumeDispersive::ErrorMagneticMaterial::~ErrorMagneticMaterial() throw () {

}

PMVolumeDispersive::PMVolumeDispersive(
 const MatId id,
 const string& name,
 const Real rEps,
 const Real rMu,
 const Real elecCond,
 const Real magnCond) : PMVolume(id, name, rEps, rMu) {
	// Adds conductivity as a permittivity pole.
	if (elecCond != 0.0) {
		complex<Real> pole(0.0);
		complex<Real> residue(elecCond/Real(2.0)/Constants::eps0, 0);
		poleResidue_.push_back(PoleResidue(pole,residue));
	}
	//
	if (magnCond != 0.0) {
	    throw ErrorMagneticMaterial();
	}
}

PMVolumeDispersive::PMVolumeDispersive(
 const MatId id,
 const string& name,
 const Real rEps,
 const Real rMu,
 const Real elecCond,
 const Real magnCond,
 const vector<PoleResidue>& poleResidue,
 const vector<PoleResidue>& drudePoleResidue)
  : PMVolume(id, name, rEps, rMu) {
	*this = PMVolumeDispersive(id, name, rEps, rMu, elecCond, magnCond);
	poleResidue_ = poleResidue;
	drudePoleResidue_ = drudePoleResidue;
}

PMVolumeDispersive::~PMVolumeDispersive() {

}

UInt PMVolumeDispersive::getDrudePoleNumber() const {
    return drudePoleResidue_.size();
}


UInt PMVolumeDispersive::getPoleNumber() const {
    return poleResidue_.size();
}

complex<Real> PMVolumeDispersive::getPole(UInt p) const {
    return poleResidue_[p].first;
}

complex<Real> PMVolumeDispersive::getResidue(UInt p) const {
    return poleResidue_[p].second;
}

complex<Real> PMVolumeDispersive::getDrudePole(UInt p) const {
    return drudePoleResidue_[p].first;
}

complex<Real> PMVolumeDispersive::getDrudeResidue(UInt p) const {
    return drudePoleResidue_[p].second;
}

bool PMVolumeDispersive::isDispersive() const {
    return true;
}

bool PMVolumeDispersive::isClassic() const {
    return isSimplyConductive();
}


bool
PMVolumeDispersive::isSimplyConductive() const {
    if (!file_.empty()) {
        return false;
    }
    return (poleResidue_.size() <= 1 && std::abs(getPole(0)) == 0);
}

void
PMVolumeDispersive::printInfo() const {
	cout << "--- PMVolumeDispersive info ---" << endl;
	PMVolume::printInfo();
	cout << "Type: " << "Dispersive material" << endl;
	cout << "Number of pole residue pairs: " << poleResidue_.size() << endl;
	cout << "# " << " re_a " << " im_a " << " re_c " << " im_c " << endl;
	for (UInt i = 0; i < poleResidue_.size(); i++) {
		cout << i << " " << getPole(i).real() << " " << getPole(i).imag()
		 << " " << getResidue(i).real()
		 << " " << getResidue(i).imag() << endl;
	}
	cout << "Number of Drude Pole Residue pairs: " <<
	 getDrudePoleNumber() << endl;
	cout << "# " << " re_a " << " im_a " << " re_c " << " im_c " << endl;
	for (UInt i = 0; i < getDrudePoleNumber(); i++) {
		cout << i << " " << getDrudePole(i).real()
		        << " " << getDrudePole(i).imag()
		        << " " << getDrudeResidue(i).real()
		        << " " << getDrudeResidue(i).imag() << endl;
	}
}

Real
PMVolumeDispersive::getElectricConductivity() const {
	if (getPoleNumber() > 1) {
		cout << endl << "WARNING @ getElectricConductivity: "
			<< "This material is dispersive and its effective permittivity "
			<< "depends on several parameters."
			<< "Returning static limit conductivity." << endl;
	}
	for (UInt i = 0; i < getPoleNumber(); i++) {
		if (std::abs(getPole(i)) == 0) {
			return getResidue(i).real() * 2.0 * Constants::eps0;
		}
	}
	return 0.0;
}

void
PMVolumeDispersive::addDrudePole(
 const complex<Real>& pole, const complex<Real>& res) {
	drudePoleResidue_.push_back(PoleResidue(pole, res));
	return;
}

void
PMVolumeDispersive::addPole(
 const complex<Real>& pole, const complex<Real>& res) {
	poleResidue_.push_back(PoleResidue(pole,res));
	return;
}

PMVolumeDispersive::PMVolumeDispersive(
        const MatId id,
        const string& name,
        const ProjectFile& file) : PMVolume(id, name) {
    file_ = file;
}

const ProjectFile PMVolumeDispersive::getFile() const {
    return file_;
}
