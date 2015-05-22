/*
 * Surfel.cpp
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz-Cabello N.
 */

#include "Surfel.h"

bool Surfel::isInto (const CVecI3Frac& coordIntFractional_){
    CVecR3 pos_ = coordIntFractional_.getRelativePos();
    for(UInt n=0; n<3; n++){
        if(n==(UInt)(normId_-1)){
            if(MathUtils::notEqual(
                    pos_.val[n],(Real)coordIntFractional_.val[n])){
                return false;
            }
        }else if ( pos_.val[n]<(Real)coordIntFractional_.val[n] ||
                pos_.val[n]>((Real)coordIntFractional_.val[n]+1.0) ){
            return false;
        }
    }
    return true;
}
