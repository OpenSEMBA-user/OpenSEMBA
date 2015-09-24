// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
        const Real rEpsInfty,
        const Real rMuInfty,
        const Real elecCond,
        const Real magnCond) : PMVolume(id, name) {
    rEpsInfty_ = rEpsInfty;
    rMuInfty_ = rMuInfty;
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
        const vector<PoleResidue>& poleResidue)
: PMVolume(id, name) {
    *this = PMVolumeDispersive(id, name, rEps, rMu, elecCond, magnCond);
    poleResidue_ = poleResidue;
}

PMVolumeDispersive::PMVolumeDispersive(
        const MatId id,
        const string& name,
        const ProjectFile& file) : PMVolume(id, name) {
    rEpsInfty_ = 1.0;
    rMuInfty_ = 1.0;
    file_ = file;
}

PMVolumeDispersive::~PMVolumeDispersive() {

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

bool PMVolumeDispersive::isDispersive() const {
    if (poleResidue_.size() > 0) {
        return true;
    }
    return false;
}

bool PMVolumeDispersive::isClassic() const {
    return isSimplyConductive();
}

bool PMVolumeDispersive::isSimplyConductive() const {
    if (!file_.empty()) {
        return false;
    }
    return (poleResidue_.size() <= 1 && std::abs(getPole(0)) == 0);
}

Real PMVolumeDispersive::getElectricConductivity() const {
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

void PMVolumeDispersive::addPole(
        const complex<Real>& pole, const complex<Real>& res) {
    poleResidue_.push_back(PoleResidue(pole,res));
    return;
}

const ProjectFile PMVolumeDispersive::getFile() const {
    return file_;
}

void PMVolumeDispersive::printInfo() const {
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
}
