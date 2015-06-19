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

Surfel::Surfel(const CVecI3& pos, const CartesianDirection& norm) : Pixel(pos) {
    normId_ = norm;
}

bool Surfel::operator ==(const Surfel& rhs) const {
    return (Pixel::operator==(rhs) && rhs.normId_ == normId_);
}

string Surfel::toStr() const {
    stringstream res;
    res << CVecI3::toStr() << " Norm: " << Pixel::toStr(normId_);
    return res.str();
}

SurfelLinels Surfel::getLinels() const {
    SurfelLinels res;
    for (UInt i = 0; i < 4; i++) {
        res[i] = getLinel(i);
    }
    return res;
}

Linel Surfel::getLinel(const UInt s) const {
    CVecI3 pos = *this;
    UInt norm(normId_-1);
    if (s == 2) {
        pos((norm+2)%3)++;
    } else if (s == 3) {
        pos((norm+1)%3)++;
    }
    UInt dir = (normId_ + (s%2)) % 3;
    return Linel(pos, CartesianDirection(dir+1));
}


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
