/*
 * FunctionLinearInterpolation.h
 *
 *  Created on: Sep 9, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_MATH_FUNCTIONLINEARINTERPOLATION_H_
#define SRC_COMMON_MATH_FUNCTIONLINEARINTERPOLATION_H_

#include <map>
#include <type_traits>
using namespace std;

#include "Function.h"
#include "ProjectFile.h"

template<class S, class T>
class FunctionLinearInterpolation: public Function<S,T> {
public:
    FunctionLinearInterpolation();
    FunctionLinearInterpolation(const vector<pair<S,T>>& xy);
    FunctionLinearInterpolation(const ProjectFile& file);
    virtual ~FunctionLinearInterpolation();

    DEFINE_CLONE(FunctionLinearInterpolation)

    T operator()(const S& arg) const;
    bool operator==(const FunctionBase& arg) const;

    void printInfo() const;
private:

    map<S,T> value_;

    void initFromFile(const ProjectFile& file);
};

typedef FunctionLinearInterpolation<Real,Real> LinearInterpolation;

#include "FunctionLinearInterpolation.hpp"

#endif /* SRC_COMMON_MATH_FUNCTIONLINEARINTERPOLATION_H_ */
