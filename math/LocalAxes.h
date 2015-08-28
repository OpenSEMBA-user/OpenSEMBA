/*
 * LocalAxes.h
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_MATH_LOCALAXES_H_
#define SRC_COMMON_MATH_LOCALAXES_H_

#include "StaMatrix.h"

class LocalAxes {
public:
    LocalAxes();
    LocalAxes(CVecR3 eulerAngles, CVecR3 origin = CVecR3());
    virtual ~LocalAxes();

    MatR33 getTransformationMatrix() const;
    const CVecR3 getEulerAngles() const;
    const CVecR3 getOrigin() const;

    MatR33 convertToGlobal(const MatR33& local) const;
    CVecR3 convertToGlobal(const CVecR3& local) const;

private:
    CVecR3 eulerAngles_;
    CVecR3 origin_;
};

#endif /* SRC_COMMON_MATH_LOCALAXES_H_ */
