/*
 * LocalAxes.h
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_MATH_LOCALAXES_H_
#define SRC_COMMON_MATH_LOCALAXES_H_

#include "StaMatrix.h"

class LocalAxes: public StaMatrix<Real,3,3> {
public:
    LocalAxes(CVecR3 eulerAngles, CVecR3 origin);
    virtual ~LocalAxes();
private:
    CVecR3 origin_;
};

#endif /* SRC_COMMON_MATH_LOCALAXES_H_ */
