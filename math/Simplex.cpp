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
 
const Polynomial<double>&
Simplex::getLagr(const unsigned int i) const {
	exit(SIMPLEX_ERROR);
}
 
const Polynomial<double>&
Simplex::getDLagr(const unsigned int i, const unsigned int f) const {
	exit(SIMPLEX_ERROR);
}
 
double
Simplex::getCda(unsigned int i, unsigned int j, unsigned int k) const {
	exit(SIMPLEX_ERROR);
}
 
inline unsigned int
Simplex::nodeIndex(unsigned int i, unsigned int j) const {
	exit(SIMPLEX_ERROR);
}
 
inline unsigned int
Simplex::cubatureNodeIndex(unsigned int i, unsigned int j) const {
	exit(SIMPLEX_ERROR);
}
 
unsigned int
Simplex::factorial(unsigned int n) const {
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}
 
Polynomial<double>
Simplex::silvesterPol(const unsigned int m, const unsigned int n) const {
    // Purpose: Generates coefficients of the R polynomial as are defined in
	// Sylvester's book page 130. These polynomials have m equispace zeros to
	// the left of m/n and none to the right. These are necessary to build
	// Lagrange's polynomial.
   	// Inputs:  Through arguments: Family member int number "m". From Simplex
	// class: Simplex order "N".
	Polynomial<double> res(1);
	res.addMonomial(1.0, 0);
	if (m != 0) {
		// Computes polynomial product.
		for (int k = 0 ; k < int(m); k++) {
			Polynomial<double> aux(1);
			aux.addMonomial(double(-k), 0);
			aux.addMonomial(double(n), 1);
			aux.removeZeros();
			res *= aux;
		}
		// Computes factorial and divides by it.
		double fact = 1.0;
		for (unsigned int k = 1; k <= m; k++)
			fact *= k;
		res /= fact;
	}
	return res;
}
 
void
Simplex::lagrangePolynomials(
 Polynomial<double>* res,
 const unsigned int n,
 const unsigned int np,
 const unsigned int nsc) const {
	// Computes Sylvester's polynomials.
	Polynomial<double> pol[n+1];
	for (unsigned int i = 0; i < (n+1); i++) {
		pol[i] = silvesterPol(i,n);
	}
	// Computes Lagrange's polynomials.
	for (unsigned int i = 0; i < np; i++) {
		for (unsigned int j = 0; j < nsc; j++) {
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
 Polynomial<double>* res,
 const unsigned int n,
 const unsigned int np,
 const unsigned int nsc) const {
	// Computes Sylvester's polynomials.
	Polynomial<double> pol[10+1];
	for (unsigned int i = 0; i < (n+1); i++)
		pol[i] = silvesterPol(i,n);
	// Computes Lagrange's polynomials.
	for (unsigned int i = 0; i < np; i++)
		for (unsigned int j = 0; j < nsc; j++)
			if (j == 0)
				res[i] = pol[cubatureNodeIndex(i,j)];
			else
				res[i] ^= pol[cubatureNodeIndex(i,j)];
}
 
double
Simplex::integrate(
  const Polynomial<double> pol,
  const unsigned int dim,
  const double sizeFactor) const {
	assert(pol.numberOfVariables() == dim + 1);
	unsigned int nsc = dim + 1;
	double sum = 0.0;
	double auxNum, auxDen;
	for (unsigned int i = 0; i < pol.numberOfMonomials(); i++) {
		auxNum = pol.monomialValue(i);
		auxDen = 0.0;
		for (unsigned int j = 0; j < nsc; j++) {
			auxNum *= factorial(pol.monomialPower(i,j));
			auxDen += pol.monomialPower(i,j);
		}
		sum += auxNum * factorial(dim) / factorial(auxDen + dim);
	}
	return (sizeFactor * sum);
}
