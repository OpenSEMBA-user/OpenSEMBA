/*
 * Voxel.cpp
 *
 *  Created on: May 19, 2015
 *      Author: mdebi
 */

#include "Voxel.h"

Voxel::Voxel(const CVecI3& cVecI3_) :
        Pixel(cVecI3_) {
}

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

VoxelLinels Voxel::getLinels() const {
    VoxelLinels res;
    VoxelSurfels surfels = getSurfels();
    for (UInt i = 0; i < surfels.size(); i++) {
        res[i*2] = surfels[i].getLinel(0);
        res[i*2+1] = surfels[i].getLinel(2);
    }
    return res;
}

VoxelSurfels Voxel::getSurfels() const {
    VoxelSurfels res;
    for (UInt i = 0; i < 6; i++) {
        const UInt dir = (i/2)%3;
        const UInt bound = i % 2;
        CVecI3 pos = *this;
        pos(dir) = pos(dir) + bound;
        res[i] = Surfel(pos, CartesianDirection(dir + 1));
    }
    return res;
}
