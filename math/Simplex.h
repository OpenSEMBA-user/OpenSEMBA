/* =============== Simplex procedures =========================================
* PURPOSE:
* - Simplex indices numeration methods.
* - Simplex Lagrange's polynomial related methods.
========================================================================= */
#ifndef SIMPLEX_H_
#define SIMPLEX_H_

#ifndef SIMPLEX_ERROR
	#define SIMPLEX_ERROR 4
#endif

#include <iostream>			// Stream I/O.
#include <vector>
#include "MathMatrix.h"
#include "CartesianVector.h"
#include "Polynomial.h"

#ifndef SIMPLEX_CUBATURE_ORDER
#	define SIMPLEX_CUBATURE_ORDER 5 // Tested to work up to 7.
#	if SIMPLEX_CUBATURE_ORDER != 5
#		warning "Simplex cubature order is not standard."
#	endif
#endif

class Simplex {
public:
	Simplex();
	virtual ~Simplex();
	virtual double
	 getCda(unsigned int i, unsigned int j, unsigned int k) const;
	virtual const Polynomial<double>&
	 getLagr(const unsigned int i) const;
	virtual const Polynomial<double>&
	 getDLagr(const unsigned int i, const unsigned int f) const;
	virtual unsigned int
	 nodeIndex(const unsigned int i, const unsigned int j) const;
	virtual unsigned int
	 cubatureNodeIndex(const unsigned int i, const unsigned int j) const;
	virtual const CartesianVector<double, 4>
	 cubatureCoordinate(const unsigned int c) const {
		 cerr << "ERROR @ cubatureCoordinate" << endl;
		 exit(SIMPLEX_ERROR);
	 }
protected:
	static const unsigned int cubatureOrder = SIMPLEX_CUBATURE_ORDER;
	Polynomial<double>
	 silvesterPol(
	  const unsigned int m,
	  const unsigned int n) const;
	void
	 lagrangePolynomials(
	  Polynomial<double>* lagr,
	  const unsigned int n,
	  const unsigned int np,
	  const unsigned int nsc) const;
	void
	 cubatureLagrangePolynomials(
	  Polynomial<double>* lagr,
	  const unsigned int n,
	  const unsigned int np,
	  const unsigned int nsc) const;
	double
     integrate(
      const Polynomial<double> pol,
      const unsigned int dimension,
      const double sizeFactor) const;
	unsigned int
	 factorial(unsigned int n) const;
};
// ========================================================================
#endif
