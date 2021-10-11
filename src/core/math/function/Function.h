

#pragma once

#include "math/Types.h"
#include "math/Constants.h"

namespace SEMBA {
namespace Math {
namespace Function {

class Base {
public:
    Base() {}
    virtual ~Base() {}

    virtual Base* clone() const = 0;

    virtual bool operator==(const Base& rhs) const = 0;
};

template<class S, class T>
class Function : public Base {
public:
    Function();
    virtual ~Function();

    virtual T operator()(const S& arg) const = 0;
    T eval(const S& arg) const;
};

} /* namespace Function */

typedef Function::Function<Real, Real> FunctionRR;

} /* namespace Math */
} /* namespace SEMBA */

#include "Function.hpp"

#ifndef SEMBA_MATH_FUNCTION_DEFINE_CLONE
#define SEMBA_MATH_FUNCTION_DEFINE_CLONE(NAME)  \
    NAME* clone() const {                       \
        return new NAME(*this);                 \
    }
#endif

