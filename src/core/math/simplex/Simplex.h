
#pragma once

#include <stdexcept>
#include <vector>

#include "math/vector/Cartesian.h"
#include "math/function/Polynomial.h"

namespace SEMBA {
namespace Math {
namespace Simplex {

class Simplex {
public:
    Simplex();
    virtual ~Simplex();
    virtual const Function::Polynomial<Real>& getLagr(
        const std::size_t i) const = 0;
    virtual const Function::Polynomial<Real>& getDLagr(
        const std::size_t i,
        const std::size_t f) const = 0;
    virtual std::vector<Real> getWeights() const = 0;

protected:

    virtual size_t nodeIndex(const std::size_t node,
                             const std::size_t coordinate) const = 0;

    Function::Polynomial<Real> silvesterPol(const std::size_t m,
                                            const std::size_t n) const;
    void lagrangePolynomials(Function::Polynomial<Real>* lagr,
                             const std::size_t n,
                             const std::size_t np,
                             const std::size_t nsc) const;

    Real integrate(const Function::Polynomial<Real> pol,
                   const std::size_t dimension,
                   const Real sizeFactor) const;
    static std::size_t factorial(std::size_t n);
};

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

