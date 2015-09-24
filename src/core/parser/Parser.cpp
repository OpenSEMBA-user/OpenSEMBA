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
// File: parser.cpp
// =============== Includes headers ===========================================
#include "Parser.h"

Parser::Parser() {
    scalingFactor_ = 1.0;
}

Parser::~Parser() {

}

CVecR3 Parser::strToCartesianVector(const string& str) {
    istringstream iss(str);
    string sub;
    CartesianVector<Real,3> res;
    for (UInt i = 0; i < 3; i++) {
        iss >> sub;
        res(i) = atof(sub.c_str());
    }
    return res;
}

bool Parser::strToBool(const string& value) {
    if (atoi(value.c_str()) == 1) {
        return true;
    } else {
        return false;
    }
}

void Parser::postReadOperations(SmbData* res) {
    if (res->mesh != NULL) {
        res->mesh->applyScalingFactor(scalingFactor_);
    }
    if (res->grid != NULL) {
        res->grid->enlarge(boundaryPadding_, boundaryMeshSize_);
        res->grid->applyScalingFactor(scalingFactor_);
    }
}
