/* =============== Polynomial procedures ======================================
 * PURPOSE:
 * -
 * -
   ========================================================================= */
#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_
// ----------- Libraries ------------------------------------------------------
#ifndef POLYNOMIAL_ERROR
	#define POLYNOMIAL_ERROR 9017
#endif
// ----------- Libraries ------------------------------------------------------
#include <iostream> // Stream I/O
#include <stdlib.h> // Included because of gcc compatibility issue.
#include <cmath>
#include <vector>
#include "MathMatrix.h"
#include "CartesianVector.h"
// ----------- Namespace ------------------------------------------------------
using namespace std;
// =========== Polynomial class ===============================================
template<class T>
class Polynomial {
public:
	Polynomial();
	Polynomial(const unsigned int nvar);
	unsigned int
	 numberOfVariables() const;
	unsigned int
	 numberOfMonomials() const;
	T
	 monomialValue(const unsigned int i) const;
	unsigned int
	 monomialPower(const unsigned int monomial, const unsigned int variable) const;
	Polynomial<T>&
	 operator=(const Polynomial<T> &param);
	// Products between polynomials assuming variables are the same.
	Polynomial<T>
	 operator*(const Polynomial<T> &param ) const;
	Polynomial<T>&
	 operator*=(const Polynomial<T> &param );
	// Products between polynomials assuming variables are different.
	Polynomial<T>
	 operator^(const Polynomial<T> &param);
	Polynomial<T>&
	 operator^=(const Polynomial<T> &param);
	Polynomial<T>&
	 operator/=(const T param );
	void
	 addMonomial(T val, vector<int> pow);
	void
	 addMonomial(T val, int pow);
	int
	 maxPower() const;
	T
	 eval(const CartesianVector<T,1>& pos) const;
	T
	 eval(const CartesianVector<T,2>& pos) const;
	T
	 eval(const CartesianVector<T,3>& pos) const;
	T
	 eval(const CartesianVector<T,4>& pos) const;
	void
	 derive(int coord);
	Polynomial<T>
	 vectorToPolynomial(T *v,  int sv, int nvar);
	Polynomial<T>
	 matrixToPolynomial(const DynMatrix<T> &param) const;
	DynMatrix<T>
	 polynomialToMatrix() const;
	void
	 removeZeros();
	void
	 printInfo() const;
protected:
	unsigned int nv; // Number of variables.
	unsigned int nm; // Number of monomials.
	vector<T> mv; // Value of constant monomials coefficients.
	vector<vector<int> > mp; // nm x nv array, containing powers of monomials.
};
// ============================================================================
#include "Polynomial.cpp"
// ============================================================================
#endif /* POLYNOMIAL_H_ */
