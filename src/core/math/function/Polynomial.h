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

	bool operator==(const FunctionBase& rhs) const;

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
