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

#include <map>
#include <string>
#include <vector>

#include "Function.h"

namespace SEMBA {
namespace Math {
namespace Function {

template<class S, class T>
class LinearInterpolation : public Function<S,T> {
public:
    LinearInterpolation();
    LinearInterpolation(const std::vector<std::pair<S,T>>& xy);
    LinearInterpolation(const std::string& file);
    virtual ~LinearInterpolation();

    SEMBA_MATH_FUNCTION_DEFINE_CLONE(LinearInterpolation)

    T operator()(const S& arg) const;
    bool operator==(const Base& arg) const;

    void printInfo() const;

private:
    std::map<S,T> value_;

    void initFromFile(const std::string& file);
};

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */

#include "LinearInterpolation.hpp"

