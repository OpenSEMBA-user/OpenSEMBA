// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.

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
    virtual void printInfo() const = 0;
};

template<class S, class T>
class Function : public Base {
public:
    Function();
    virtual ~Function();

    virtual T operator()(const S& arg) const = 0;
    T eval(const S& arg) const;

    void printInfo() const;
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

