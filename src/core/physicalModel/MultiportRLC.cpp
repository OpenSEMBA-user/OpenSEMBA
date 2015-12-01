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
 * PMMultiportRLC.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "PMMultiportRLC.h"

PMMultiportRLC::PMMultiportRLC(
        const MatId id,
        const string name,
        const PMMultiport::Type type,
        const Real resistance,
        const Real inductance,
        const Real capacitance) : PMMultiport(id, name) {
    type_ = type;
    R_ = resistance;
    L_ = inductance;
    C_ = capacitance;
}

PMMultiportRLC::~PMMultiportRLC() {

}

Real
PMMultiportRLC::getR() const {
    return R_;
}

Real
PMMultiportRLC::getL() const {
    return L_;
}

Real
PMMultiportRLC::getC() const {
    return C_;
}

void
PMMultiportRLC::printInfo() const {
    cout<< " --- Multiport RLC Info --- " << endl;
    PMMultiport::printInfo();
    cout<< " Type: " << getTypeStr() << endl;
    cout<< " Resistance: " << R_ << endl;
    cout<< " Inductance: " << L_ << endl;
    cout<< " Capacitance: " << C_ << endl;
}
