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
#ifndef SEMBA_MATH_SIMPLEX_SIMPLEX_H_
#define SEMBA_MATH_SIMPLEX_SIMPLEX_H_

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
    virtual std::size_t nodeIndex(const std::size_t i, 
                                  const std::size_t j) const = 0;
protected:
    Function::Polynomial<Real> silvesterPol(const std::size_t m,
                                            const std::size_t n) const;
    void lagrangePolynomials(Function::Polynomial<Real>* lagr,
                             const std::size_t n,
                             const std::size_t np,
                             const std::size_t nsc) const;
    void cubatureLagrangePolynomials(Function::Polynomial<Real>* lagr,
                                     const std::size_t n,
                                     const std::size_t np,
                                     const std::size_t nsc) const;
    Real integrate(const Function::Polynomial<Real> pol,
                   const std::size_t dimension,
                   const Real sizeFactor) const;
    std::size_t factorial(std::size_t n) const;
};

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#endif /* SEMBA_MATH_SIMPLEX_SIMPLEX_H_ */
