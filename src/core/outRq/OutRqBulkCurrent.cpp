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
 * OutRqBulkCurrent.cpp
 *
 *  Created on: 20 de ago. de 2015
 *      Author: Daniel
 */

#include <outRq/OutRqBulkCurrent.h>

OutRqBulkCurrent::OutRqBulkCurrent(const Domain& domain,
                                   const string& name,
                                   const GroupElements<Elem>& elem,
                                   const CartesianAxis& dir,
                                   const UInt& skip)
:   Domain(domain),
    OutRqBase(bulkCurrentElectric, name),
    GroupElements<const Elem>(elem) {

    dir_ = dir;
    skip_ = skip;
}

OutRqBulkCurrent::OutRqBulkCurrent(const OutRqBulkCurrent& rhs)
:   Domain(rhs),
    OutRqBase(rhs),
    GroupElements<const Elem>(rhs) {

    dir_ = rhs.dir_;
    skip_ = rhs.skip_;
}

OutRqBulkCurrent::~OutRqBulkCurrent() {

}

bool OutRqBulkCurrent::hasSameProperties(const OutRqBase& rhs) const {
    if(!OutRqBase::hasSameProperties(rhs)) {
        return false;
    }
    const OutRqBulkCurrent* rhsPtr = rhs.castTo<OutRqBulkCurrent>();
    bool hasSameProperties = true;
    hasSameProperties &= dir_ == rhsPtr->dir_;
    hasSameProperties &= skip_ == rhsPtr->skip_;
    return hasSameProperties;
}

CartesianAxis OutRqBulkCurrent::getDir() const {
    return dir_;
}

UInt OutRqBulkCurrent::getSkip() const {
    return skip_;
}

