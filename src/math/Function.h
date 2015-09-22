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
