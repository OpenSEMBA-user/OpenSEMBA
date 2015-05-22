/*
 * Linel.cpp
 *
 *  Created on: May 19, 2015
 *      Author: mdebi
 */

#include "Linel.h"



bool Linel::isInto (const CVecI3Fractional& coordIntFractional_){
    CVecR3 pos_ = coordIntFractional_.getRelativePos();
    for(UInt n=0; n<3; n++){
        if(n==(UInt)(dirId_-1)){
            if ( pos_.val[n]<(Real)coordIntFractional_[n] ||
                    pos_.val[n]>((Real)coordIntFractional_[n]+1.0) ){
                return false;
            }
        }else if(MathUtils::notEqual(pos_.val[n],(Real)coordIntFractional_[n])){
            return false;
        }
    }
    return true;
}



