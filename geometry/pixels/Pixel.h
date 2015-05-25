/*
 * Pixel.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

#ifndef SRC_MESHER_FDTD_MESHCONF_PIXEL_H_
#define SRC_MESHER_FDTD_MESHCONF_PIXEL_H_

#include "geometry/CVecI3Fractional.h"
#include "base/class/ClassBase.h"

class Pixel : public CVecI3, public ClassBase {
public:
    Pixel():CVecI3(){};
    Pixel(const CVecI3& cVecI3_): CVecI3(cVecI3_){};
    virtual ~Pixel(){};

    DEFINE_CLONE(Pixel);

    void printInfo() const;
};

#endif /* SRC_MESHER_FDTD_MESHCONF_PIXEL_H_ */
