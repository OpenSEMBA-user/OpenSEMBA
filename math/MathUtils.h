/*
 * MathUtils.h
 *
 *  Created on: Jul 21, 2013
 *      Author: luis
 */

#ifndef MATHUTILS_H_
#define MATHUTILS_H_

#include <utility>
#include <vector>
#include <math.h>
#include <complex>
#include "CartesianVector.h"

using namespace std;

namespace MathUtils {

	template<class T, Int D>
	CartesianVector<complex<T>,D>
	 convertToComplex(
	  const CartesianVector<T,D>& rhs);

	template<class T, Int D>
	vector<CartesianVector<complex<T>,D> >
	 convertToComplex(
	  const vector<CartesianVector<T,D> >& rhs);

	template<class T, Int D>
	void
	convertToComplex(
	 CartesianVector<complex<T>,D>* res,
	 const CartesianVector<T,D>* rhs,
	 const UInt vS);

	template<class T>
	vector<Real>
	 logspace(
	  const pair<Real, Real>& rangeExponents,
	  const T nPoints);

	template<class T>
	vector<T>
	 linspace(
	  const pair<T,T>& range,
	  const UInt nPoints);

	template<class T>
	T
	 meanDifference(const vector<T>& vec);

	template<class T>
	T
	 mean(const vector<T>& vec);

	template<class T>
	T
	 sum(const vector<T>& vec);

	template<class T>
	complex<T>
	 getDFT(
	  const Real frequency,
	  const vector<T>& time,
	  const vector<T>& signal);

	template<class T, Int D>
	CartesianVector<complex<T>,D>
	 getDFT(
	  const Real frequency,
	  const vector<T>& time,
	  const vector<CartesianVector<T,D> >& signal);
}

#include "MathUtils.hpp"

#endif /* MATHUTILS_H_ */
