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
 * PMSurface.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMSurfaceSIBC.h"

PMSurfaceSIBC::PMSurfaceSIBC(
        const MatId id,
        const string& name,
        const MatR22& Zinfinite,
        const MatR22& Zstatic,
        const vector<Real>& pole,
        const vector<MatR22 >& Z)
: PMSurface(id, name) {
    ZInfinity_ = Zinfinite;
    ZStatic_ = Zstatic;
    pole_ = pole;
    Z_ = Z;
}

PMSurfaceSIBC::~PMSurfaceSIBC() {

}

PMSurfaceSIBC&
PMSurfaceSIBC::operator=(
        const PMSurfaceSIBC &rhs) {
    if (this == &rhs) {
        return *this;
    }
    PMSurface::operator=(rhs);
    ZInfinity_ = rhs.ZInfinity_;
    ZStatic_ = rhs.ZStatic_;
    pole_ = rhs.pole_;
    Z_ = rhs.Z_;
    file_ = rhs.file_;
    return *this;
}

void
PMSurfaceSIBC::setZInfinity(const MatR22& ZInf) {
    ZInfinity_ = ZInf;
}

void
PMSurfaceSIBC::setZStatic(const MatR22& ZSta) {
    ZStatic_ = ZSta;
}

void
PMSurfaceSIBC::addPole(const Real vP, const MatR22& Z) {
    pole_.push_back(vP);
    Z_.push_back(Z);
}

void
PMSurfaceSIBC::printInfo() const {
    cout << "--- Physical model information ---"   << endl;
    cout << "Surface Impedance Boundary Condition" << endl;
    PMSurface::printInfo();
    cout << "Impedance @ infininite freq.:";
    ZInfinity_.printInfo();
    cout << "Impedance @ zero freq.:";
    ZStatic_.printInfo();
    if (pole_.size()) {
        cout << "Number of poles: " << pole_.size() << endl;
        cout << "# Pole Z11 Z12 Z21 Z22 " << endl;
    }
    for (register UInt i = 0; i < pole_.size(); i++) {
        cout << i << " "
                << pole_[i] << " "
                << Z_[i](0,0) << " " << Z_[i](0,1) << " "
                << Z_[i](1,0) << " " << Z_[i](1,1) << endl;
    }
}

UInt PMSurfaceSIBC::getNumberOfPoles() const {
    return pole_.size();
}

PMSurfaceSIBC::PMSurfaceSIBC(
        const MatId id,
        const string& name,
        const ProjectFile& file) : PMSurface(id, name) {
    file_ = file;
}

const ProjectFile PMSurfaceSIBC::getFile() const {
    return file_;
}
