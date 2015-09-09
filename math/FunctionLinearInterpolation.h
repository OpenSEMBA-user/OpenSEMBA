/*
 * FunctionLinearInterpolation.h
 *
 *  Created on: Sep 9, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_MATH_FUNCTIONLINEARINTERPOLATION_H_
#define SRC_COMMON_MATH_FUNCTIONLINEARINTERPOLATION_H_

#include <map>
using namespace std;

#include "Function.h"

template<class S, class T>
class FunctionLinearInterpolation: public Function<S,T> {
public:
    FunctionLinearInterpolation();
    FunctionLinearInterpolation(const vector<pair<S,T>>& xy);
    virtual ~FunctionLinearInterpolation();

    T operator()(const S& arg) const;
private:

};

#include "FunctionLinearInterpolation.hpp"

#endif /* SRC_COMMON_MATH_FUNCTIONLINEARINTERPOLATION_H_ */
