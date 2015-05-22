/*
 * Pixel.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

#ifndef SRC_MESHER_FDTD_MESHCONF_PIXEL_H_
#define SRC_MESHER_FDTD_MESHCONF_PIXEL_H_

#include "geometry/CVecI3Fractional.h"
#include "geometry/elements/Element.h"

//class CartesianDirections{
//public:
//    CartesianDirections(){} //: dirNode(0),dirX(1), dirY(2), dirZ(3){}
//    static const UInt dirNode = 0;
//    static const UInt dirX  = 1;
//    static const UInt dirY  = 2;
//    static const UInt dirZ  = 3;
//};

class Pixel : public CVecI3 {
public:
    Pixel():CVecI3(){};
    Pixel(const CVecI3& cVecI3_): CVecI3(cVecI3_){};
    virtual ~Pixel(){};
//    virtual bool isInto (const CVecI3Fractional& rhs) const=0;
};

#endif /* SRC_MESHER_FDTD_MESHCONF_PIXEL_H_ */
