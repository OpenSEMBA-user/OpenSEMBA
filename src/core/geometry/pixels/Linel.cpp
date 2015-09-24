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
 * Linel.cpp
 *
 *  Created on: May 19, 2015
 *      Author: mdebi
 */

#include "Linel.h"

Linel::Linel() {
    dirId_ = CartesianDirection::dirNode;
}

Linel::~Linel() {
}

bool Linel::isInto (const CVecI3Fractional& fracPos)const{
    CVecR3 pos = fracPos.getScalePos(*this);
    for(UInt n=0; n<3; n++){
        if(n==(UInt)(dirId_-1)){
            if (pos(n)<0.0 || pos(n)>1.0 ){
                return false;
            }
        }else if (!MathUtils::equal(pos(n),0.0)){
            return false;
        }
    }
    return true;
}

Linel::Linel(const CVecI3& pos, const CartesianDirection& dir) :
    Pixel(pos) {
    dirId_ = dir;
}

bool Linel::operator ==(const Linel& rhs) const {
    return (Pixel::operator==(rhs) && rhs.dirId_ == dirId_);
}

string Linel::toStr() const {
    stringstream res;
    res << CVecI3::toStr() << " Dir: " << Pixel::toStr(dirId_);
    return res.str();
}

bool Linel::operator<(const Linel& rhs) const {
    if (Pixel::operator<(rhs)) {
        return true;
    }
    if (Pixel::operator==(rhs)) {
        return dirId_ < rhs.dirId_;
    }
    return false;
}
