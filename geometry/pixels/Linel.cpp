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

bool Linel::isInto (const CVecI3Fractional& coordIntFractional_){
    CVecR3 pos_ = coordIntFractional_.getRelativePos();
    for(UInt n=0; n<3; n++){
        if(n==(UInt)(dirId_-1)){
            if ( pos_.val[n]<(Real)coordIntFractional_[n] ||
                    pos_.val[n]>((Real)coordIntFractional_[n]+1.0) ){
                return false;
            }
        }else if(MathUtils::notEqual(pos_.val[n],(Real)coordIntFractional_[n])){
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
