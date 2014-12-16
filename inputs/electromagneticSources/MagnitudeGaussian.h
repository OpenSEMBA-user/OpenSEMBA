/*
 * MagnitudeGaussian.h
 *
 *  Created on: Dec 16, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDEGAUSSIAN_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDEGAUSSIAN_H_

#include "Magnitude.h"
#include <math.h>

class MagnitudeGaussian: public Magnitude {
public:
    typedef enum {
        gaussian,
        gaussianDerivative
    } type;
    MagnitudeGaussian();
    virtual ~MagnitudeGaussian();
    double
     evaluate(const double time) const;
private:
    double delay_;
    double spread_;
    double freq_;
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDEGAUSSIAN_H_ */
