/*
 * Function.h
 *
 *  Created on: Sep 9, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_MATH_FUNCTION_H_
#define SRC_COMMON_MATH_FUNCTION_H_

#include <iostream>
#include <math.h>
using namespace std;

#include "FunctionBase.h"
#include "RealUtils.h"
#include "Types.h"

template<class S, class T>
class Function : public FunctionBase {
public:
    Function();
    virtual ~Function();

    virtual T operator()(const S& arg) const = 0;
    T eval(const S& arg) const;

    void printInfo() const;
};

typedef Function<Real, Real> FunctionRR;

#include "Function.hpp"

#endif /* SRC_COMMON_MATH_FUNCTION_H_ */
