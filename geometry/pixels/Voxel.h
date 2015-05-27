/*
 * Voxel.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

#ifndef SRC_MESHER_FDTD_MESHCONF_VOXEL_H_
#define SRC_MESHER_FDTD_MESHCONF_VOXEL_H_

#include "Surfel.h"

class Voxel: public Pixel {
public:
    Voxel();
    Voxel(const CVecI3& cVecI3_): Pixel(cVecI3_){};
    virtual ~Voxel();
    bool isInto (const CVecI3Fractional& rhs) const;
    array<Linel,12> getLinels() const;
    array<Surfel,6> getSurfels() const;
};

#endif /* SRC_MESHER_FDTD_MESHCONF_VOXEL_H_ */
