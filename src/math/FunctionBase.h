/*
 * FunctionBase.h
 *
 *  Created on: Sep 9, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_MATH_FUNCTIONBASE_H_
#define SRC_COMMON_MATH_FUNCTIONBASE_H_

#include "base/class/ClassBase.h"

class FunctionBase : public virtual ClassBase {
public:
    FunctionBase();
    virtual ~FunctionBase();

    virtual bool operator==(const FunctionBase& rhs) const = 0;
    virtual void printInfo() const = 0;
};

#endif /* SRC_COMMON_MATH_FUNCTIONBASE_H_ */
