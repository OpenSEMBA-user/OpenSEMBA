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
 * Dipole.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#include "../sources/Dipole.h"

Dipole::Dipole(const Magnitude* magnitude,
               const GroupElements<Vol>& elem,
               Real length,
               CVecR3 orientation,
               CVecR3 position)
:   EMSourceBase(magnitude),
    GroupElements<const Vol>(elem) {

    length_ = length;
    orientation_ = orientation;
    position_ = position;
//    gaussDelay_ = magnitude.getDelay();
//    spreadSqrt2_ = magnitude.getSpread() * sqrt(2.0);
}

Dipole::Dipole(const Dipole& rhs)
:   EMSourceBase(rhs),
    GroupElements<const Vol>(rhs) {

    length_ = rhs.length_;
    orientation_ = rhs.orientation_;
    position_ = rhs.position_;
    gaussDelay_ = rhs.gaussDelay_;
    spreadSqrt2_ = rhs.spreadSqrt2_;
}

Dipole::~Dipole() {

}

bool Dipole::hasSameProperties(const EMSourceBase& rhs) const {
    if(!EMSourceBase::hasSameProperties(rhs)) {
        return false;
    }
    const Dipole* rhsPtr = rhs.castTo<Dipole>();
    bool hasSameProperties = true;
    hasSameProperties &= length_ == rhsPtr->length_;
    hasSameProperties &= orientation_ == rhsPtr->orientation_;
    hasSameProperties &= position_ == rhsPtr->position_;
    hasSameProperties &= gaussDelay_ == rhsPtr->gaussDelay_;
    hasSameProperties &= spreadSqrt2_ == rhsPtr->spreadSqrt2_;
    return hasSameProperties;
}

const string& Dipole::getName() const {
    const static string res = "Dipole";
    return res;
}

void Dipole::printInfo() const {
    cout << " ---- Dipole information ---- " << endl;
    EMSourceBase::printInfo();
    cout << " - Length: " << length_ << endl;
    cout << " - Orientation vector:";;
    orientation_.printInfo();
    cout << endl;
    cout << " - Position vector:";
    position_.printInfo();
    cout << endl;
}
