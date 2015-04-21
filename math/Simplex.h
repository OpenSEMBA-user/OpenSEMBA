/* =============== Simplex procedures =========================================
* PURPOSE:
* - Simplex indices numeration methods.
* - Simplex Lagrange's polynomial related methods.
========================================================================= */
#ifndef SIMPLEX_H_
#define SIMPLEX_H_

#include <iostream>
#include <vector>
using namespace std;

#include "StaMatrix.h"
#include "DynMatrix.h"
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
	virtual Real
	 getCda(UInt i, UInt j, UInt k) const;
	virtual const Polynomial<Real>&
	 getLagr(const UInt i) const;
	virtual const Polynomial<Real>&
	 getDLagr(const UInt i, const UInt f) const;
	virtual UInt
	 nodeIndex(const UInt i, const UInt j) const;
	virtual UInt
	 cubatureNodeIndex(const UInt i, const UInt j) const;
	virtual const CartesianVector<Real, 4>
	 cubatureCoordinate(const UInt c) const {
		 cerr << endl << "ERROR @ cubatureCoordinate" << endl;
		 exit(EXIT_FAILURE);
	 }
protected:
	static const UInt cubatureOrder = SIMPLEX_CUBATURE_ORDER;
	Polynomial<Real>
	 silvesterPol(
	  const UInt m,
	  const UInt n) const;
	void
	 lagrangePolynomials(
	  Polynomial<Real>* lagr,
	  const UInt n,
	  const UInt np,
	  const UInt nsc) const;
	void
	 cubatureLagrangePolynomials(
	  Polynomial<Real>* lagr,
	  const UInt n,
	  const UInt np,
	  const UInt nsc) const;
	Real
     integrate(
      const Polynomial<Real> pol,
      const UInt dimension,
      const Real sizeFactor) const;
	UInt
	 factorial(UInt n) const;
};
// ========================================================================
#endif
