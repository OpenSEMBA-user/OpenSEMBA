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
