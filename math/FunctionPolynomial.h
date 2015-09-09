#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include "Function.h"
#include "MathMatrix.h"
#include "CartesianVector.h"

template<class T>
class FunctionPolynomial : public Function<T,T>{
public:
	FunctionPolynomial();
	FunctionPolynomial(const UInt nvar);
	virtual ~FunctionPolynomial();

    DEFINE_CLONE(FunctionPolynomial);

	UInt numberOfVariables() const;
	UInt numberOfMonomials() const;

	T monomialValue(const UInt i) const;
	UInt monomialPower(const UInt monomial, const UInt variable) const;

	FunctionPolynomial<T>& operator=(const FunctionPolynomial<T> &param);

	// Product between polynomials assuming variables are the same.
	FunctionPolynomial<T> operator*(const FunctionPolynomial<T> &param ) const;
	FunctionPolynomial<T>& operator*=(const FunctionPolynomial<T> &param);

	// Product between polynomials assuming variables are different.
	FunctionPolynomial<T> operator^(const FunctionPolynomial<T> &param);
	FunctionPolynomial<T>& operator^=(const FunctionPolynomial<T> &param);

	FunctionPolynomial<T>& operator/=(const T param );
	void addMonomial(T val, vector<Int> pow);
	void addMonomial(T val, Int pow);
	Int maxPower() const;

	T operator()(const T&) const;
	T eval(const CartesianVector<T,1>& pos) const;
	T eval(const CartesianVector<T,2>& pos) const;
	T eval(const CartesianVector<T,3>& pos) const;
	T eval(const CartesianVector<T,4>& pos) const;

	void derive(Int coord);

	FunctionPolynomial<T> vectorToPolynomial(T *v,  Int sv, Int nvar);
	FunctionPolynomial<T> matrixToPolynomial(const DynMatrix<T> &param) const;
	DynMatrix<T> polynomialToMatrix() const;
	void removeZeros();

	void printInfo() const;
private:
	UInt nv_; // Number of variables.
	UInt nm_; // Number of monomials.
	vector<T> mv_; // Value of constant monomials coefficients.
	vector<vector<Int>> mp_; // nm x nv array, containing powers of monomials.
};

#include "FunctionPolynomial.hpp"

#endif /* POLYNOMIAL_H_ */
