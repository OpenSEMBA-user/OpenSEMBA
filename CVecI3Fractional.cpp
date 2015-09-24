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
 * CoordIntFractional.cpp
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

using namespace std;

#include "CVecI3Fractional.h"

CVecI3Fractional::CVecI3Fractional() {
}

CVecI3Fractional& CVecI3Fractional::operator =(
        const CVecI3Fractional & rhs) {
    this->CVecI3::operator =(rhs);
    len_ = rhs.len_;
    return *this;
}

bool CVecI3Fractional::operator ==(const CVecI3Fractional& rhs) const {
    if(CVecI3::operator !=(rhs)){
        return false;
    }
    for (unsigned int n = 0; n < 3; ++n) {
        if(len_(n) != rhs.len_(n)){
            return false;
        }
    }
    return true;
}


CartesianDirection CVecI3Fractional::getDirBase() const {
    UInt rang = this->getRangeBase();
    if (rang == 1) {
        for (unsigned int n = 0; n < 3; ++n) {
            if (len_[n] > MathUtils::tolerance) {
                return CartesianDirection(n + 1);
            }
        }
    } else if (rang == 2) {
        for (unsigned int n = 0; n < 3; ++n) {
            if (len_[n] < MathUtils::tolerance) {
                return CartesianDirection(n + 1);
            }
        }
    }
    return dirNode;
}

UInt CVecI3Fractional::getRangeBase() const {
    unsigned int rang;
    rang = 0;
    for (unsigned int n = 0; n < 3; ++n) {
        if (len_[n] > MathUtils::tolerance) {
            ++rang;
        }
    }
    return rang;
}

CVecR3 CVecI3Fractional::getScalePos() const {
    CVecR3 retPos;
    for (unsigned int n = 0; n < 3; ++n) {
        retPos.val[n] = (Real)val[n] + len_.val[n];
    }
    return retPos;
}

CVecR3 CVecI3Fractional::getScalePos(const CVecI3 origin) const {
    CVecR3 retPos;
    for (unsigned int n = 0; n < 3; ++n) {
        retPos(n) = (Real)((*this)(n)-origin(n)) + this->len_(n);
    }
    return retPos;
}

string CVecI3Fractional::toStr() const {
    return CVecI3::toStr() + " len: " + len_.toStr();
}



CVecR3 CVecI3Fractional::meanRelativePoint(const CVecI3Fractional& rhs) const {
    return (len_ + rhs.len_)*0.5;
}

CVecR3 CVecI3Fractional::getRelativePosNearestNode() const {
    CVecR3 nearestPos;
    for(UInt dir=0; dir<3; ++dir){
        nearestPos(dir) = 0.0;
        if(len_(dir)>0.5){
            nearestPos(dir) = 1.0;
        }
    }
    return nearestPos;
}

void CVecI3Fractional::join(CVecI3Fractional& rhs) {
    for(UInt dir=0; dir<3; ++dir){
        if(MathUtils::equal(len_(dir),0.0)){
            rhs.len_(dir) = 0.0;
        }else if(MathUtils::equal(rhs.len_(dir),0.0)){
            len_(dir) = 0.0;
        }else{
           len_(dir) = rhs.len_(dir);
        }
    }
}

CVecI3Fractional& CVecI3Fractional::reduceTopology() {
    CVecR3 posNode = getRelativePosNearestNode();
    CVecR3 dst =  (posNode - len_).abs();
    Real dstMin = 1e20;
    Int minDir = -1;
    for(UInt dir=0; dir<3; ++dir){
        if(len_(dir)!=0.0){
            if(dstMin>dst[dir]){
                dstMin = dst(dir);
                minDir = dir;
            }
        }
    }
    if(minDir!=-1){
        len_(minDir) = posNode(minDir);
        reduceCoords();
    }
    return *this;
}

CVecI3Fractional& CVecI3Fractional::reduceTopology(const UInt range){
    UInt rangeLoc = getRangeBase();
    UInt n = 0;
    while(rangeLoc>range && rangeLoc>0 && n <3){
        reduceTopology();
        rangeLoc = getRangeBase();
        ++n;
    }
    return *this;
}

CVecI3Fractional& CVecI3Fractional::reduceCoords() {
    for(UInt dir=0; dir<3; ++dir){
        if(len_(dir)>=(1.0-MathUtils::tolerance)){
            len_(dir)= 0.0;
            (*this)(dir)++;
//        }else if (len_(dir)<(-1.0)*MathUtils::tolerance){
//            len_(dir) = 0.0;
//            val[dir]--;
        }else if (len_(dir) < MathUtils::tolerance){
            len_(dir)=0.0;
        }
    }

    return *this;
}

CVecI3Fractional& CVecI3Fractional::CVecI3Fractional::move(
        CVecI3Fractional& rhs,
        const bool forceProject,
        bool& canBeMoved) const {
    canBeMoved = true;
    CVecR3 len;
    for (UInt dir = 0; dir < 3; ++dir) {
        if (MathUtils::equal(len_(dir), 0.0)) {
            len(dir) = 0.0;
        } else {
            if (MathUtils::equal(rhs.len_(dir), 0.0)) {
                canBeMoved = false;
            } else {
                len(dir) = len_(dir);
            }
        }
    }
    if (canBeMoved || forceProject) {
        rhs.len_ = len;
    }
    return rhs;
}
