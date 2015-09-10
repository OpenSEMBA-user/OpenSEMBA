/*
 * Gaussian.h
 *
 *  Created on: Sep 9, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_MATH_FUNCTIONGAUSSIAN_H_
#define SRC_COMMON_MATH_FUNCTIONGAUSSIAN_H_

#include "Function.h"

class FunctionGaussian: public Function<Real,Real> {
public:
    FunctionGaussian();
    FunctionGaussian(const Real spread,
                      const Real delay,
                      const Real freq = 0.0);
    FunctionGaussian(const FunctionGaussian& rhs);
    virtual ~FunctionGaussian();

    DEFINE_CLONE(FunctionGaussian);

    Real operator()(const Real&) const;

    bool operator==(const FunctionBase& rhs) const;

    Real getDelay() const;
    Real getFreq() const;
    Real getSpread() const;

    void printInfo() const;

private:
    Real spread_;
    Real delay_;
    Real freq_;
};

typedef FunctionGaussian Gaussian;

#endif /* SRC_COMMON_MATH_FUNCTIONGAUSSIAN_H_ */
