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

#ifndef SEMBA_MATH_SIMPLEX_LINE_H_
#define SEMBA_MATH_SIMPLEX_LINE_H_

#include "math/matrix/Static.h"

#include "Simplex.h"

namespace SEMBA {
namespace Math {
namespace Simplex {

template <size_t N>
class Line : public Simplex {
public:
    static const std::size_t faces = 2;
    static const std::size_t dimension = 1;
    static const std::size_t nsc = 2;
    static const std::size_t nfp = 1;
    static constexpr std::size_t np = (N + 1);
    static constexpr Real sizeFactor = 1.0;

    typedef Vector::Cartesian<size_t,nsc> Index;

    Line();
    std::size_t vertex(const std::size_t) const;
    std::size_t sideVertex(const std::size_t f, const std::size_t i) const;
    std::size_t sideNode( const std::size_t f, const std::size_t i) const;

    std::size_t nodeIndex(const std::size_t i, const std::size_t j) const;

    const Function::Polynomial<Real>& getLagr(
            const std::size_t node) const;
    const Function::Polynomial<Real>& getDLagr(
            const std::size_t node, const std::size_t simplex) const;

    std::vector<Real> getWeights() const;

    void  printInfo() const;

private:
    std::array<Index,np> indices;
    Matrix::Static<Int,faces,nfp> sideNodes;

    Function::Polynomial<Real> lagr[np];
    Function::Polynomial<Real> dLagr[np][faces];

    std::array<Vector::Cartesian<Real,nsc>, np> nodePositions;
    std::array<Real,np>                         weights;

    Matrix::Static<Int, nfp, np> RMatrix(const std::size_t s) const;
    Matrix::Static<Int,  np, np> PMatrix(const std::size_t s) const;
};

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Line.hpp"

#endif /* SEMBA_MATH_SIMPLEX_LINE_H_ */
