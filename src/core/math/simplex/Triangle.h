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
#ifndef SEMBA_MATH_SIMPLEX_TRIANGLE_H_
#define SEMBA_MATH_SIMPLEX_TRIANGLE_H_

#include "math/vector/Cartesian.h"
#include "math/matrix/Static.h"
#include "math/function/Polynomial.h"

#include <algorithm>

#include "Simplex.h"

namespace SEMBA {
namespace Math {
namespace Simplex {

template <size_t N>
class Triangle : public Simplex {
public:
    static const std::size_t faces = 3;
    static const std::size_t nsc = 3;
    static const std::size_t dimension = 2;
    static const std::size_t vertices = 3;
    static constexpr std::size_t np  = (N + 1)*(N + 2) / 2;
    static constexpr std::size_t nfp = N + 1;
    static constexpr Real sizeFactor = 1.0 / 2.0;

    typedef Vector::Cartesian<size_t,nsc> Index;

    Triangle();
    std::size_t vertex(const std::size_t i) const;
    std::size_t sideVertex(const std::size_t f, const std::size_t i) const;
    std::size_t sideNode  (const std::size_t f, const std::size_t i) const;

    std::size_t nodeIndex(const std::size_t i, const std::size_t j) const;
    Vector::Cartesian<Real,nsc> coordinate(const std::size_t i) const;
    const Function::Polynomial<Real>& getLagr(const std::size_t i) const;
    const Function::Polynomial<Real>& getDLagr(const std::size_t i,
                                               const std::size_t f) const;
    std::vector<Real> getWeights() const {
        std::vector<Real> res(np);
        std::copy_n(weights.begin(), np, res.begin());
        return res;
    }

    void printInfo() const;

    static Matrix::Dynamic<Int> PMatrix(const std::size_t n,
                                        const std::size_t s);
private:
    Vector::Cartesian<Real,nsc> cPos[np];
    Matrix::Static<Int,nfp,np>  R[faces];
    Index nId[np];
    Matrix::Static<Int,faces,nfp> sNId;
    Function::Polynomial<Real> lagr[np];
    Function::Polynomial<Real> dLagr[np][faces];
    std::array<Real,np> weights;

    Matrix::Static<Int,nfp,np> RMatrix(
            const std::size_t s) const;
    void buildNodeIndices();
    void buildSideNodeIndices();
    static size_t numberOfNodes(size_t order);
    void buildCubaturePositionsAndWeights();
    void buildCubatureLagrange();
};



} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Triangle.hpp"

#endif /* SEMBA_MATH_SIMPLEX_TRIANGLE_H_ */
