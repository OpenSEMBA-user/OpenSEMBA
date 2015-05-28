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

bool Linel::isInto (const CVecI3Fractional& fracPos){
    CVecR3 pos = fracPos.getRelativePos();
    for(UInt n=0; n<3; n++){
        if(n==(UInt)(dirId_-1)){
            if ( pos.val[n]<(Real)fracPos[n] ||
                    pos.val[n]>((Real)fracPos[n]+1.0) ){
                return false;
            }
        }else if(MathUtils::notEqual(pos.val[n],(Real)fracPos[n])){
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
