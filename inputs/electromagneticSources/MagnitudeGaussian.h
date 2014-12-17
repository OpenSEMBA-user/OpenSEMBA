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
    MagnitudeGaussian(
            const double spread,
            const double delay,
            const double freq = 0.0);
    virtual ~MagnitudeGaussian();
    double
     evaluate(const double time) const;
    void
     printInfo() const;
private:
    double spread_;
    double delay_;
    double freq_;
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDEGAUSSIAN_H_ */
