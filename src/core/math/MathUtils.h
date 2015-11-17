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
 * MathUtils.h
 *
 *  Created on: Jul 21, 2013
 *      Author: luis
 */

#ifndef MATHUTILS_H_
#define MATHUTILS_H_

#include <cmath>
#include <complex>
#include <utility>
#include <vector>
using namespace std;

#include "Constants.h"
#include "CartesianVector.h"

namespace MathUtils {

template<class T, Int D>
CartesianVector<complex<T>,D> convertToComplex(
        const CartesianVector<T,D>& rhs);

template<class T, Int D>
vector<CartesianVector<complex<T>,D> > convertToComplex(
        const vector<CartesianVector<T,D> >& rhs);

template<class T, Int D>
void convertToComplex(
        CartesianVector<complex<T>,D>* res,
        const CartesianVector<T,D>* rhs,
        const UInt vS);

template<class T>
vector<Real> logspace(
        const pair<Real, Real>& rangeExponents,
        const T nPoints);

template<class T>
vector<T> linspace(
        const pair<T,T>& range,
        const UInt nPoints);

template<class T>
T meanDifference(const vector<T>& vec);

template<class T>
T mean(const vector<T>& vec);

template<class T>
T sum(const vector<T>& vec);

template<class T>
complex<T> getDFT(
        const Real frequency,
        const vector<T>& time,
        const vector<T>& signal);

template<class T, Int D>
CartesianVector<complex<T>,D> getDFT(
        const Real frequency,
        const vector<T>& time,
        const vector<CartesianVector<T,D> >& signal);

template<class T>
bool complexLT(complex<T> a, complex<T> b);
}

#include "MathUtils.hpp"

#endif /* MATHUTILS_H_ */
