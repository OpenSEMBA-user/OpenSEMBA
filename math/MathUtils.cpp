/*
 * MathUtils.cpp
 *
 *  Created on: Jul 21, 2013
 *      Author: luis
 */

#ifndef MATHUTILS_H_
	#include "MathUtils.h"
#endif


template<class T, int D>
inline CartesianVector<complex<T>,D>
MathUtils::convertToComplex(
 const CartesianVector<T,D>& rhs) {
	CartesianVector<complex<T>,D> res;
	for (int d = 0; d < D; d++) {
		res(d) = complex<T>(rhs(d), (T) 0.0);
	}
	return res;
}

template<class T, int D>
inline void
MathUtils::convertToComplex(
 CartesianVector<complex<T>,D> *res,
 const CartesianVector<T,D> *rhs,
 const unsigned int vS) {
	for (unsigned int i = 0; i < vS; i++) {
		res[i] = convertToComplex(rhs[i]);
	}
}


template<class T, int D>
inline vector<CartesianVector<complex<T>,D> >
MathUtils::convertToComplex(
 const vector<CartesianVector<T,D> >& rhs) {
	const unsigned int vS = rhs.size();
	vector<CartesianVector<complex<T>,D> > res(vS);
	for (unsigned int i = 0; i < vS; i++) {
		res[i] = convertToComplex(rhs[i]);
	}
	return res;
}

template<class T>
vector<double>
MathUtils::logspace(
 const pair<double,double>& rangeExponents,
 const T nPoints) {
	vector<double> res;
	const double base = (double) 10;
	res.reserve(nPoints);
	if (nPoints == 1) {
		res.push_back(pow(base, rangeExponents.second));
	} else {
		double jump =
		 (rangeExponents.second - rangeExponents.first)
    	 / (double) (nPoints - 1);
		for (unsigned int i = 0; i < nPoints; i++) {
			res.push_back(
			 pow(base, rangeExponents.first + (double) i * jump));
		}
	}
	return res;
}

template<class T>
vector<T>
MathUtils::linspace(
 const pair<T,T>& range,
 const unsigned int nPoints) {
	vector<T> res;
	res.reserve(nPoints);
	if (nPoints == 1) {
		res.push_back(range.second);
	} else {
		T jump = (range.second - range.first) / (T) (nPoints - 1);
		for (unsigned int i = 0; i < nPoints; i++) {
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
	const unsigned int vS = vec.size();
	vector<T> diff(vS-1, 0.0);
	for (unsigned int i = 1; i < vS; i++) {
		diff[i-1] = vec[i] - vec[i-1];
	}
	return mean(diff);
}

template<class T>
T
MathUtils::mean(const vector<T>& vec) {
	const unsigned int vS = vec.size();
	return sum(vec) / vS;
}

template<class T>
T
MathUtils::sum(const vector<T>& vec) {
	const unsigned int vS = vec.size();
	T res = 0.0;
	for (unsigned int i = 0; i < vS; i++) {
		res += vec[i];
	}
	return res;
}

template<class T>
complex<T>
MathUtils::getDFT(
 const double frequency,
 const vector<T>& time,
 const vector<T>& signal) {
	const unsigned int N = signal.size();
	const complex<T> constPart(0.0, -(T)(2.0 * M_PI * frequency));
	complex<T> res(0.0, 0.0);
	for (unsigned int i = 0; i < N; i++) {
		res += signal[i] * exp(constPart * time[i]);
	}
	res /= (double) N;
	return conj(res);
}

template<class T, int D>
CartesianVector<complex<T>,D>
MathUtils::getDFT(
 const double frequency,
 const vector<T>& time,
 const vector<CartesianVector<T,D> >& signal) {
	CartesianVector<complex<T>,D> res;
	const unsigned int N = signal.size();
	vector<T> auxSignal(N);
	for (int d = 0; d < D; d++) {
		for (unsigned int i = 0; i < N; i++) {
			auxSignal[i] = signal[i](d);
		}
		res(d) = getDFT(frequency, time, auxSignal);
	}
	return res;
}
