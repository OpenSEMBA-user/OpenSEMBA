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
 * MathUtils.cpp
 *
 *  Created on: Jul 21, 2013
 *      Author: luis
 */

#ifndef MATHUTILS_H_
	#include "MathUtils.h"
#endif


template<class T, Int D>
inline CartesianVector<complex<T>,D>
MathUtils::convertToComplex(
 const CartesianVector<T,D>& rhs) {
	CartesianVector<complex<T>,D> res;
	for (Int d = 0; d < D; d++) {
		res(d) = complex<T>(rhs(d), (T) 0.0);
	}
	return res;
}

template<class T, Int D>
inline void
MathUtils::convertToComplex(
 CartesianVector<complex<T>,D> *res,
 const CartesianVector<T,D> *rhs,
 const UInt vS) {
	for (UInt i = 0; i < vS; i++) {
		res[i] = convertToComplex(rhs[i]);
	}
}


template<class T, Int D>
inline vector<CartesianVector<complex<T>,D> >
MathUtils::convertToComplex(
 const vector<CartesianVector<T,D> >& rhs) {
	const UInt vS = rhs.size();
	vector<CartesianVector<complex<T>,D> > res(vS);
	for (UInt i = 0; i < vS; i++) {
		res[i] = convertToComplex(rhs[i]);
	}
	return res;
}

template<class T>
vector<Real>
MathUtils::logspace(
 const pair<Real,Real>& rangeExponents,
 const T nPoints) {
	vector<Real> res;
	const Real base = (Real) 10;
	res.reserve(nPoints);
	if (nPoints == 1) {
		res.push_back(pow(base, rangeExponents.second));
	} else {
		Real jump =
		 (rangeExponents.second - rangeExponents.first)
    	 / (Real) (nPoints - 1);
		for (UInt i = 0; i < nPoints; i++) {
			res.push_back(
			 pow(base, rangeExponents.first + (Real) i * jump));
		}
	}
	return res;
}

template<class T>
vector<T>
MathUtils::linspace(
 const pair<T,T>& range,
 const UInt nPoints) {
	vector<T> res;
	res.reserve(nPoints);
	if (nPoints == 1) {
		res.push_back(range.second);
	} else {
		T jump = (range.second - range.first) / (T) (nPoints - 1);
		for (UInt i = 0; i < nPoints; i++) {
			res.push_back(range.first + (T) i * jump);
		}
	}
	return res;
}

template<class T>
T
MathUtils::meanDifference(
 const vector<T>& vec) {
	if (vec.size() == 1) {
		return (T) 0.0;
	}
	const UInt vS = vec.size();
	vector<T> diff(vS-1, 0.0);
	for (UInt i = 1; i < vS; i++) {
		diff[i-1] = vec[i] - vec[i-1];
	}
	return mean(diff);
}

template<class T>
T
MathUtils::mean(const vector<T>& vec) {
	const UInt vS = vec.size();
	return sum(vec) / vS;
}

template<class T>
T
MathUtils::sum(const vector<T>& vec) {
	const UInt vS = vec.size();
	T res = 0.0;
	for (UInt i = 0; i < vS; i++) {
		res += vec[i];
	}
	return res;
}

template<class T>
complex<T>
MathUtils::getDFT(
 const Real frequency,
 const vector<T>& time,
 const vector<T>& signal) {
	const UInt N = signal.size();
	const complex<T> constPart(0.0, -(T)(2.0 * M_PI * frequency));
	complex<T> res(0.0, 0.0);
	for (UInt i = 0; i < N; i++) {
		res += signal[i] * exp(constPart * time[i]);
	}
	res /= (Real) N;
	return conj(res);
}

template<class T, Int D>
CartesianVector<complex<T>,D>
MathUtils::getDFT(
 const Real frequency,
 const vector<T>& time,
 const vector<CartesianVector<T,D> >& signal) {
	CartesianVector<complex<T>,D> res;
	const UInt N = signal.size();
	vector<T> auxSignal(N);
	for (Int d = 0; d < D; d++) {
		for (UInt i = 0; i < N; i++) {
			auxSignal[i] = signal[i](d);
		}
		res(d) = getDFT(frequency, time, auxSignal);
	}
	return res;
}
