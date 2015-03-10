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
 
const Polynomial<Real>&
Simplex::getLagr(const UInt i) const {
	exit(SIMPLEX_ERROR);
}
 
const Polynomial<Real>&
Simplex::getDLagr(const UInt i, const UInt f) const {
	exit(SIMPLEX_ERROR);
}
 
Real
Simplex::getCda(UInt i, UInt j, UInt k) const {
	exit(SIMPLEX_ERROR);
}
 
inline UInt
Simplex::nodeIndex(UInt i, UInt j) const {
	exit(SIMPLEX_ERROR);
}
 
inline UInt
Simplex::cubatureNodeIndex(UInt i, UInt j) const {
	exit(SIMPLEX_ERROR);
}
 
UInt
Simplex::factorial(UInt n) const {
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}
 
Polynomial<Real>
Simplex::silvesterPol(const UInt m, const UInt n) const {
    // Purpose: Generates coefficients of the R polynomial as are defined in
	// Sylvester's book page 130. These polynomials have m equispace zeros to
	// the left of m/n and none to the right. These are necessary to build
	// Lagrange's polynomial.
   	// Inputs:  Through arguments: Family member Int number "m". From Simplex
	// class: Simplex order "N".
	Polynomial<Real> res(1);
	res.addMonomial(1.0, 0);
	if (m != 0) {
		// Computes polynomial product.
		for (Int k = 0 ; k < Int(m); k++) {
			Polynomial<Real> aux(1);
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
 Polynomial<Real>* res,
 const UInt n,
 const UInt np,
 const UInt nsc) const {
	// Computes Sylvester's polynomials.
	Polynomial<Real> pol[n+1];
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
 Polynomial<Real>* res,
 const UInt n,
 const UInt np,
 const UInt nsc) const {
	// Computes Sylvester's polynomials.
	Polynomial<Real> pol[10+1];
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
  const Polynomial<Real> pol,
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
