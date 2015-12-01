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
// File: Simplex.cpp
// =============== Includes headers ===========================================
#ifndef SIMPLEX_H_
	#include "Simplex.h"
#endif

// =============== Simplex ====================================================

Simplex::Simplex() {

}
 
Simplex::~Simplex() {

}
 
const FunctionPolynomial<Real>&
Simplex::getLagr(const UInt i) const {
	exit(EXIT_FAILURE);
}
 
const FunctionPolynomial<Real>&
Simplex::getDLagr(const UInt i, const UInt f) const {
	exit(EXIT_FAILURE);
}
 
Real
Simplex::getCda(UInt i, UInt j, UInt k) const {
	exit(EXIT_FAILURE);
}
 
inline UInt
Simplex::nodeIndex(UInt i, UInt j) const {
	exit(EXIT_FAILURE);
}
 
inline UInt
Simplex::cubatureNodeIndex(UInt i, UInt j) const {
	exit(EXIT_FAILURE);
}
 
UInt
Simplex::factorial(UInt n) const {
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}
 
FunctionPolynomial<Real>
Simplex::silvesterPol(const UInt m, const UInt n) const {
    // Purpose: Generates coefficients of the R polynomial as are defined in
	// Sylvester's book page 130. These polynomials have m equispace zeros to
	// the left of m/n and none to the right. These are necessary to build
	// Lagrange's polynomial.
   	// Inputs:  Through arguments: Family member Int number "m". From Simplex
	// class: Simplex order "N".
	FunctionPolynomial<Real> res(1);
	res.addMonomial(1.0, 0);
	if (m != 0) {
		// Computes polynomial product.
		for (Int k = 0 ; k < Int(m); k++) {
			FunctionPolynomial<Real> aux(1);
			aux.addMonomial(Real(-k), 0);
			aux.addMonomial(Real(n), 1);
			aux.removeZeros();
			res *= aux;
		}
		// Computes factorial and divides by it.
		Real fact = 1.0;
		for (UInt k = 1; k <= m; k++)
			fact *= k;
		res /= fact;
	}
	return res;
}
 
void
Simplex::lagrangePolynomials(
 FunctionPolynomial<Real>* res,
 const UInt n,
 const UInt np,
 const UInt nsc) const {
	// Computes Sylvester's polynomials.
	FunctionPolynomial<Real> pol[n+1];
	for (UInt i = 0; i < (n+1); i++) {
		pol[i] = silvesterPol(i,n);
	}
	// Computes Lagrange's polynomials.
	for (UInt i = 0; i < np; i++) {
		for (UInt j = 0; j < nsc; j++) {
			if (j == 0) {
				res[i] = pol[nodeIndex(i,j)];
			} else {
				res[i] ^= pol[nodeIndex(i,j)];
			}
		}
	}
}
 
void
Simplex::cubatureLagrangePolynomials(
 FunctionPolynomial<Real>* res,
 const UInt n,
 const UInt np,
 const UInt nsc) const {
	// Computes Sylvester's polynomials.
	FunctionPolynomial<Real> pol[10+1];
	for (UInt i = 0; i < (n+1); i++)
		pol[i] = silvesterPol(i,n);
	// Computes Lagrange's polynomials.
	for (UInt i = 0; i < np; i++)
		for (UInt j = 0; j < nsc; j++)
			if (j == 0)
				res[i] = pol[cubatureNodeIndex(i,j)];
			else
				res[i] ^= pol[cubatureNodeIndex(i,j)];
}
 
Real
Simplex::integrate(
  const FunctionPolynomial<Real> pol,
  const UInt dim,
  const Real sizeFactor) const {
	assert(pol.numberOfVariables() == dim + 1);
	UInt nsc = dim + 1;
	Real sum = 0.0;
	Real auxNum, auxDen;
	for (UInt i = 0; i < pol.numberOfMonomials(); i++) {
		auxNum = pol.monomialValue(i);
		auxDen = 0.0;
		for (UInt j = 0; j < nsc; j++) {
			auxNum *= factorial(pol.monomialPower(i,j));
			auxDen += pol.monomialPower(i,j);
		}
		sum += auxNum * factorial(dim) / factorial(auxDen + dim);
	}
	return (sizeFactor * sum);
}
