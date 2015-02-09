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

//typedef unsigned int uint;;

using namespace std;

namespace MathUtils {

	template<class T, int D>
	CartesianVector<complex<T>,D>
	 convertToComplex(
	  const CartesianVector<T,D>& rhs);

	template<class T, int D>
	vector<CartesianVector<complex<T>,D> >
	 convertToComplex(
	  const vector<CartesianVector<T,D> >& rhs);

	template<class T, int D>
	void
	convertToComplex(
	 CartesianVector<complex<T>,D>* res,
	 const CartesianVector<T,D>* rhs,
	 const unsigned int vS);

	template<class T>
	vector<double>
	 logspace(
	  const pair<double, double>& rangeExponents,
	  const T nPoints);

	template<class T>
	vector<T>
	 linspace(
	  const pair<T,T>& range,
	  const unsigned int nPoints);

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
	  const double frequency,
	  const vector<T>& time,
	  const vector<T>& signal);

	template<class T, int D>
	CartesianVector<complex<T>,D>
	 getDFT(
	  const double frequency,
	  const vector<T>& time,
	  const vector<CartesianVector<T,D> >& signal);
}

#include "MathUtils.hpp"

#endif /* MATHUTILS_H_ */
