/*
 * Surfel.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz-Cabello N.
 */

#ifndef SRC_MESHER_FDTD_MESHCONF_SURFEL_H_
#define SRC_MESHER_FDTD_MESHCONF_SURFEL_H_

#include "Pixel.h"

class Surfel: public Pixel {
public:
    Surfel(){normId_ = CartesianDirection::dirNode;};
    Surfel(const CVecI3& cVecI3_): Pixel(cVecI3_){normId_ = CartesianDirection::dirNode;}

    virtual ~Surfel(){};

    bool isInto (const CVecI3Frac& intFractional);

    CartesianDirection  getNormId ()const{return normId_;}
    CartesianDirection& NormId (){return normId_;}
    void setNormId (const CartesianDirection& _normId){normId_ = _normId;}
private:
    CartesianDirection normId_;
};

#endif /* SRC_MESHER_FDTD_MESHCONF_SURFEL_H_ */
