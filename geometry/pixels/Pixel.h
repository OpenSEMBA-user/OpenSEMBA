/*
 * Pixel.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

#ifndef SRC_MESHER_FDTD_MESHCONF_PIXEL_H_
#define SRC_MESHER_FDTD_MESHCONF_PIXEL_H_

#include "math/CVecI3Fractional.h"
#include "base/class/ClassBase.h"
#include <array>

class Pixel : public CVecI3 {
public:
    Pixel():CVecI3(){};
    Pixel(const CVecI3& cVecI3_): CVecI3(cVecI3_){};
    virtual ~Pixel(){};

    bool operator<(const Pixel& rhs) const;

    void printInfo() const;

    static string toStr(const CartesianDirection& dir);
};

#endif /* SRC_MESHER_FDTD_MESHCONF_PIXEL_H_ */
