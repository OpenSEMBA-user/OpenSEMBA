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
 * Pixel.cpp
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

#include "Pixel.h"

void Pixel::printInfo() const {
    cout << " -- Pixel info -- " << endl;
    cout << "Pos: " << CVecI3::toStr() << endl;
}

string Pixel::toStr(const CartesianDirection& dir) {
    switch (dir) {
    case CartesianDirection::dirX:
        return "X";
    case CartesianDirection::dirY:
        return "Y";
    case CartesianDirection::dirZ:
        return "Z";
    default:
        return "Node";
    }
}

bool Pixel::operator<(const Pixel& rhs) const {
    for (UInt i = 0; i < 3; i++) {
        if (val[i] < rhs.val[i]) {
            return true;
        }
        if (val[i] > rhs.val[i]) {
            return false;
        }
    }
    return false;
}
