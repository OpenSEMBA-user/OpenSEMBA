/*
 * Voxel.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

#ifndef SRC_MESHER_FDTD_MESHCONF_VOXEL_H_
#define SRC_MESHER_FDTD_MESHCONF_VOXEL_H_

#include "Surfel.h"

typedef array<Linel,12> VoxelLinels;
typedef array<Surfel,6> VoxelSurfels;

class Voxel: public Pixel {
public:
    Voxel();
    Voxel(const CVecI3& cVecI3_);
    virtual ~Voxel();
    bool isInto (const CVecI3Fractional& rhs) const;
    VoxelLinels getLinels() const;
    VoxelSurfels getSurfels() const;


};

#endif /* SRC_MESHER_FDTD_MESHCONF_VOXEL_H_ */
