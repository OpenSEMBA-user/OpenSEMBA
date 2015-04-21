/*
 * MagnitudeGaussian.h
 *
 *  Created on: Dec 16, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDEGAUSSIAN_H_
#define SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDEGAUSSIAN_H_

#include <cmath>
using namespace std;

#include "Magnitude.h"

class MagnitudeGaussian: public virtual Magnitude {
public:
    typedef enum {
        gaussian,
        gaussianDerivative
    } type;
    MagnitudeGaussian();
    MagnitudeGaussian(const Real spread,
                      const Real delay,
                      const Real freq = 0.0);
    MagnitudeGaussian(const MagnitudeGaussian& rhs);
    virtual ~MagnitudeGaussian();

    DEFINE_CLONE(MagnitudeGaussian);

    Real getDelay() const;
    Real getFreq() const;
    Real getSpread() const;

    Real evaluate(const Real time) const;

    void printInfo() const;

private:
    Real spread_;
    Real delay_;
    Real freq_;
};

#endif /* SRC_COMMON_INPUTS_ELECTROMAGNETICSOURCES_MAGNITUDEGAUSSIAN_H_ */
