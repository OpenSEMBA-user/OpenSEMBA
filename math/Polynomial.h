#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include "MathMatrix.h"
#include "CartesianVector.h"

template<class T>
class Polynomial {
public:
	Polynomial();
	Polynomial(const UInt nvar);
	UInt numberOfVariables() const;
	UInt numberOfMonomials() const;
	T monomialValue(const UInt i) const;
	UInt monomialPower(const UInt monomial, const UInt variable) const;
	Polynomial<T>& operator=(const Polynomial<T> &param);

	// Product between polynomials assuming variables are the same.
	Polynomial<T> operator*(const Polynomial<T> &param ) const;
	Polynomial<T>& operator*=(const Polynomial<T> &param);

	// Product between polynomials assuming variables are different.
	Polynomial<T> operator^(const Polynomial<T> &param);
	Polynomial<T>& operator^=(const Polynomial<T> &param);

	Polynomial<T>& operator/=(const T param );
	void addMonomial(T val, vector<Int> pow);
	void addMonomial(T val, Int pow);
	Int maxPower() const;
	T eval(const CartesianVector<T,1>& pos) const;
	T eval(const CartesianVector<T,2>& pos) const;
	T eval(const CartesianVector<T,3>& pos) const;
	T eval(const CartesianVector<T,4>& pos) const;
	void derive(Int coord);
	Polynomial<T> vectorToPolynomial(T *v,  Int sv, Int nvar);
	Polynomial<T> matrixToPolynomial(const DynMatrix<T> &param) const;
	DynMatrix<T> polynomialToMatrix() const;
	void removeZeros();
	void printInfo() const;
private:
	 // Number of variables.
	UInt nv_;

	// Number of monomials.
	UInt nm_;

	 // Value of constant monomials coefficients.
	vector<T> mv_;

	// nm x nv array, containing powers of monomials. a_
	vector<vector<Int>> mp_;
};

#include "Polynomial.hpp"

#endif /* POLYNOMIAL_H_ */
