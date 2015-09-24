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
 * Nodal.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#include "../sources/SourceOnLine.h"

SourceOnLine::SourceOnLine(const Magnitude* magnitude,
                           const GroupElements<Lin>& elem,
                           const Type& sourceType,
                           const Hardness& sourceHardness)
:   EMSourceBase(magnitude),
    GroupElements<const Lin>(elem) {

	type_ = sourceType;
    hardness_ = sourceHardness;
}

SourceOnLine::SourceOnLine(const SourceOnLine& rhs)
:   EMSourceBase(rhs),
    GroupElements<const Lin>(rhs) {

    type_ = rhs.type_;
    hardness_ = rhs.hardness_;
}

SourceOnLine::~SourceOnLine() {

}

bool SourceOnLine::hasSameProperties(const EMSourceBase& rhs) const {
    if(!EMSourceBase::hasSameProperties(rhs)) {
        return false;
    }
    const SourceOnLine* rhsPtr = rhs.castTo<SourceOnLine>();
    bool hasSameProperties = true;
    hasSameProperties &= type_ == rhsPtr->type_;
    hasSameProperties &= hardness_ == rhsPtr->hardness_;
    return hasSameProperties;
}

const string& SourceOnLine::getName() const {
    const static string res = "SourceOnLine";
    return res;
}

SourceOnLine::Type SourceOnLine::getType() const {
	return type_;
}

SourceOnLine::Hardness SourceOnLine::getHardness() const {
	return hardness_;
}

void SourceOnLine::printInfo() const {
	cout<< " --- Nodal info --- " << endl;
	EMSourceBase::printInfo();
	cout<< "Type: " << getTypeStr() << endl;
    cout<< "Hardness: " << getHardnessStr() << endl;
}

string SourceOnLine::getTypeStr() const {
	switch (type_) {
	case electric:
		return "Electric Field";
	default:
		return "Magnetic Field";
	}
}

string SourceOnLine::getHardnessStr() const {
	switch (type_) {
	case hard:
		return "Hard";
	default:
		return "Soft";
	}
}
