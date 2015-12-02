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
