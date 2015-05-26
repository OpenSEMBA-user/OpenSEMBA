/*
 * Voxel.cpp
 *
 *  Created on: May 19, 2015
 *      Author: mdebi
 */

#include "Voxel.h"

Voxel::Voxel() {}

Voxel::~Voxel() {

}

bool Voxel::isInto (const CVecI3Fractional& coordIntFractional_) const{
    CVecR3 pos_ = coordIntFractional_.getRelativePos();
    for(UInt n=0; n<3; n++){
        if( pos_[n]<(Real)this->val[n] || pos_[n]>((Real)(this->val[n])+1.0) ){
            return false;
        }
    }
    return true;
}

array<Linel, 12> Voxel::getLinels() const {
}

array<Surfel, 6> Voxel::getSurfels() const {
}
