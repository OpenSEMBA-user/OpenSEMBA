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

#include "base/error/ErrorNotImplemented.h"

#include "StaMatrix.h"
#include "DynMatrix.h"
#include "CartesianVector.h"
#include "FunctionPolynomial.h"

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
    virtual Real getCda(UInt i, UInt j, UInt k) const;
    virtual const FunctionPolynomial<Real>& getLagr(
            const UInt i) const;
    virtual const FunctionPolynomial<Real>& getDLagr(
            const UInt i,
            const UInt f) const;
    virtual UInt nodeIndex(const UInt i, const UInt j) const;
    virtual UInt cubatureNodeIndex(const UInt i, const UInt j) const;
    virtual const CVecR4 cubatureCoordinate(const UInt c) const {
        throw ErrorNotImplemented("Simplex::cubatureCoordinate");
    }
protected:
    static const UInt cubatureOrder = SIMPLEX_CUBATURE_ORDER;
    FunctionPolynomial<Real> silvesterPol(
            const UInt m,
            const UInt n) const;
    void lagrangePolynomials(
            FunctionPolynomial<Real>* lagr,
            const UInt n,
            const UInt np,
            const UInt nsc) const;
    void cubatureLagrangePolynomials(
            FunctionPolynomial<Real>* lagr,
            const UInt n,
            const UInt np,
            const UInt nsc) const;
    Real integrate(
            const FunctionPolynomial<Real> pol,
            const UInt dimension,
            const Real sizeFactor) const;
    UInt factorial(UInt n) const;
};
// ========================================================================
#endif
