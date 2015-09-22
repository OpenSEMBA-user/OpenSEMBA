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
 * Surfel.cpp
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz-Cabello N.
 */

#include "Surfel.h"

bool Surfel::isInto (const CVecI3Frac& coordIntFractional)const{
    CVecR3 pos = coordIntFractional.getScalePos((CVecI3)(*this));
    for(UInt n=0; n<3; n++){
        if(n==(UInt)(normId_-1)){
            if(!MathUtils::equal(pos(n),0.0)){
                return false;
            }
        }else if ( pos(n)<0.0 || pos(n)>1.0 ){
            return false;
        }
    }
    return true;
}

bool Surfel::operator ==(const Surfel& rhs) const {
    return (Pixel::operator==(rhs) && rhs.normId_ == normId_);
}

string Surfel::toStr() const {
    stringstream res;
    res << CVecI3::toStr() << " Norm: " << Pixel::toStr(normId_);
    return res.str();
}

//SurfelLinels Surfel::getLinels() const {
//    SurfelLinels res;
//    for (UInt i = 0; i < 4; i++) {
//        res[i] = getLinel(i);
//    }
//    return res;
//}
//
//Linel Surfel::getLinel(const UInt s) const {
//    CVecI3 pos = *this;
//    UInt norm(normId_-1);
//    if (s == 2) {
//        pos((norm+2)%3)++;
//    } else if (s == 3) {
//        pos((norm+1)%3)++;
//    }
//    UInt dir = (normId_ + (s%2)) % 3;
//    return Linel(pos, CartesianDirection(dir+1));
//}


SurfelPixels Surfel::getPixels() const{
    SurfelPixels ret;
    UInt counter = 0;
    UInt dirId = getNormId() -1;
    for(UInt n=0; n<2; ++n){
        for(UInt m=0; m<2; ++m){
            CVecI3 offset;
            offset(dirId)       = 0;
            offset((dirId+1)%3) = n;
            offset((dirId+2)%3) = m;
            ret[counter]        = (CVecI3)*this +offset;
            ++counter;
        }
    }
    return ret;
}

bool Surfel::operator<(const Surfel& rhs) const {
    if (Pixel::operator<(rhs)) {
        return true;
    }
    if (Pixel::operator==(rhs)) {
        return normId_ < rhs.normId_;
    }
    return false;
}
