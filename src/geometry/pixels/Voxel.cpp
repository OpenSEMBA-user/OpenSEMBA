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

bool Voxel::isInto (const CVecI3Fractional& coordIntFractional) const{
    CVecR3 pos = coordIntFractional.getScalePos(*this);
    for(UInt n=0; n<3; n++){
        if (pos(n)<0.0 || pos(n)>1.0 ){
            return false;
        }
    }
    return true;
}

//VoxelLinels Voxel::getLinels() const {
//    VoxelLinels res;
//    VoxelSurfels surfels = getSurfels();
//    for (UInt i = 0; i < surfels.size(); i++) {
//        res[i*2] = surfels[i].getLinel(0);
//        res[i*2+1] = surfels[i].getLinel(2);
//    }
//    return res;
//}
//
//VoxelSurfels Voxel::getSurfels() const {
//    VoxelSurfels res;
//    for (UInt i = 0; i < 6; i++) {
//        const UInt dir = (i/2)%3;
//        const UInt bound = i % 2;
//        CVecI3 pos = *this;
//        pos(dir) = pos(dir) + bound;
//        res[i] = Surfel(pos, CartesianDirection(dir + 1));
//    }
//    return res;
//}

bool Voxel::operator<(const Voxel& rhs) const {
    return Pixel::operator<(rhs);
}
