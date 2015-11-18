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
#include "OutRq.h"

OutRqBase::OutRqBase() {
}

OutRqBase::OutRqBase(const Type outputType,
                     const string& name) {
    name_ = name;
    outputType_ = outputType;
}

OutRqBase::OutRqBase(const OutRqBase& rhs) {
    name_ = rhs.name_;
    outputType_ = rhs.outputType_;
}

OutRqBase::~OutRqBase() {

}

bool OutRqBase::hasSameProperties(const OutRqBase& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    bool hasSameProperties = true;
    hasSameProperties &= getName() == rhs.getName();
    hasSameProperties &= getOutputType() == rhs.getOutputType();
    hasSameProperties &= Domain::operator==(rhs);
    return hasSameProperties;
}

const string& OutRqBase::getName() const {
    return name_;
}

OutRqBase::Type OutRqBase::getOutputType() const {
    return outputType_;
}

Domain OutRqBase::getDomain() const {
    return *this;
}


void OutRqBase::printInfo() const {
    cout<< "Name: " << name_.c_str() << endl;
    cout<< "Type: " << getTypeStr() << endl;
    Domain::printInfo();
}

string OutRqBase::getTypeStr() const {
    switch (outputType_) {
    case electric:
        return "Electric field";
    case magnetic:
        return "Magnetic field";
    case electricFieldNormals:
        return "Electric field normals";
    case magneticFieldNormals:
        return "Magnetic field normals";
    case powerDensity:
        return "Power density";
    case power:
        return "Power";
    case current:
        return "Current";
    case voltage:
        return "Voltage";
    default:
        return "Undefined";
    }
}
