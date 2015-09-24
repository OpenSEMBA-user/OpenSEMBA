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
 * PMMultilayer.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: luis
 */

#include "PMSurfaceMultilayer.h"

PMSurfaceMultilayer::ErrorIncompatibleSizes::ErrorIncompatibleSizes()
:   Error("PMSurfaceMultilayer: Incompatible sizes of layers parameters. ") {

}

PMSurfaceMultilayer::ErrorIncompatibleSizes::~ErrorIncompatibleSizes() throw () {

}

PMSurfaceMultilayer::PMSurfaceMultilayer(const MatId id,
                                         const string& name,
                                         const vector<Real>& thickness,
                                         const vector<Real>& relPermittivity,
                                         const vector<Real>& relPermeability,
                                         const vector<Real>& elecCond,
                                         const vector<Real>& magnCond)
:   PMSurface(id, name) {
    thickness_ = thickness;
    relPermittivity_ = relPermittivity;
    relPermeability_ = relPermeability;
    elecCond_ = elecCond;
    magnCond_ = magnCond;
    const UInt nLayers = thickness_.size();
    if (relPermittivity_.size() != nLayers ||
        relPermeability_.size() != nLayers ||
        elecCond_.size() != nLayers ||
        magnCond_.size() != nLayers) {
        throw ErrorIncompatibleSizes();
    }
}

UInt PMSurfaceMultilayer::getNumberOfLayers() const {
    return thickness_.size();
}

string
PMSurfaceMultilayer::printLayer(const UInt i) const {
    assert(i < getNumberOfLayers());
    stringstream ss;
    ss << elecCond_[i] << " " << relPermittivity_[i]*Constants::eps0 << " "
    << relPermeability_[i] * Constants::mu0 << " " << magnCond_[i] << " "
    << thickness_[i];
    return string(ss.str());
}

Real
PMSurfaceMultilayer::getThickness(const UInt i) const {
    return thickness_[i];
}

Real PMSurfaceMultilayer::getPermittivity(const UInt i) const {
    return relPermittivity_[i] * Constants::eps0;
}

Real PMSurfaceMultilayer::getPermeability(const UInt i) const {
    return relPermeability_[i] * Constants::mu0;
}

Real PMSurfaceMultilayer::getElecCond(const UInt i) const {
    return elecCond_[i];
}

Real PMSurfaceMultilayer::getMagnCond(const UInt i) const {
    return magnCond_[i];
}

void
PMSurfaceMultilayer::printInfo() const {
    cout << " --- PMSurfaceMultilayer info ---" << endl;
    PMSurface::printInfo();
    cout << "Number of layers: " << getNumberOfLayers() << endl;
    cout <<
    "#, Thickness, Permittivity, Permeability, ElecCond, MagnCond" << endl;
    for (UInt i = 0; i < getNumberOfLayers(); i++) {
        cout<< i << ": "
        << thickness_[i] << " "
        << relPermittivity_[i] << " "
        << relPermeability_[i] << " "
        << elecCond_[i] << " "
        << magnCond_[i] << endl;
    }
    cout << " --- End of PMSurfaceMultilayer info ---" << endl;
}
