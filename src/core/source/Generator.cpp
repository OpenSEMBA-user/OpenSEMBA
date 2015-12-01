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
 * Generator.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#include "../sources/Generator.h"

Generator::Generator() {
    type_ = current;
    hardness_ = soft;
}

Generator::Generator(const Magnitude* magnitude,
                     GroupElements<Nod>& elems,
                     const Type& generatorType,
                     const Hardness& hardness)
:   EMSourceBase(magnitude),
    GroupElements<const Nod>(elems) {

    type_ = generatorType;
    hardness_ = hardness;
}

Generator::Generator(const Generator& rhs)
:   EMSourceBase(rhs),
    GroupElements<const Nod>(rhs) {

    type_ = rhs.type_;
    hardness_ = rhs.hardness_;
}

Generator::~Generator() {

}

bool Generator::hasSameProperties(const EMSourceBase& rhs) const {
    if(!EMSourceBase::hasSameProperties(rhs)) {
        return false;
    }
    const Generator* rhsPtr = rhs.castTo<Generator>();
    bool hasSameProperties = true;
    hasSameProperties &= type_ == rhsPtr->type_;
    hasSameProperties &= hardness_ == rhsPtr->hardness_;
    return hasSameProperties;
}

const string& Generator::getName() const {
    const static string res = "Generator";
    return res;
}

Generator::Type
Generator::getType() const {
    return type_;
}

void
Generator::printInfo() const {
    cout<< " --- Generator info --- " << endl;
    EMSourceBase::printInfo();
    cout<< "Type: " << getTypeStr() << endl;
}

string
Generator::getTypeStr() const {
    switch (type_) {
    case voltage:
        return "Voltage";
    default:
        return "Current";
    }
}
