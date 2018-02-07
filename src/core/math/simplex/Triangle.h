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
// ----------- Compiler directives ----------------------------------------
// ----------- Libraries --------------------------------------------------
#include "math/vector/Cartesian.h"
#include "math/matrix/Static.h"
#include "math/function/Polynomial.h"

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
    static constexpr std::size_t np  = ((N + 1)*(N + 2) / 2);
    static constexpr std::size_t nfp = (N + 1);
    Real cw[np];
    Vector::Cartesian<Real,nsc> cPos[np];
    // Rotation and extraction matrices.
    Matrix::Static<Int,np,np> P[faces];
    Matrix::Static<Int,nfp,np> R[faces];
    Vector::Cartesian<Int,nsc> nId[np];
    Matrix::Static<Int,faces,nfp> sNId;
    Function::Polynomial<Real> lagr[np];
    Function::Polynomial<Real> dLagr[np][faces];
    // weight: This is the relative weight of a node. Is computed
    // integrating over the simplex area. The sum of all must be one.
    // Weights can be used to obtain the result of a integral knowing the
    // function values on its nodal positions.
    Real w[np];
    static const Real sizeFactor;
    Real ca[np][np];
    Real cda[np][faces][np];
    Matrix::Static<Real,np,np> cwaa[np];
    Matrix::Static<Real,np,np> cwada[np][faces];
    Triangle();
    std::size_t vertex(const std::size_t) const;
    std::size_t sideVertex(const std::size_t f, const std::size_t i) const;
    std::size_t nodeIndex(const std::size_t i, const std::size_t j) const;
    std::size_t cubatureNodeIndex(const std::size_t i,
                                  const std::size_t j) const;
    std::size_t sideNode(const std::size_t f, const std::size_t i) const;
    Vector::Cartesian<Real,3> coordinate(const std::size_t i) const;
    const Function::Polynomial<Real>& getLagr(const std::size_t i) const;
    const Function::Polynomial<Real>& getDLagr(const std::size_t i,
                                               const std::size_t f) const;
    Matrix::Static<Real,np,np> getMassMatrix() const;
    Matrix::Dynamic<Int> PMatrix(const std::size_t n,
                                 const std::size_t s) const;
    void printInfo() const;
private:
    std::size_t numberOfNodes(const std::size_t order) const;
    Matrix::Static<Int,nfp,np> RMatrix(
            const std::size_t s) const;
    void buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
                          const std::size_t order,
                          const std::size_t nNodes) const;
    void buildSideNodeIndices();
    Int numberOfNodes(Int order) const ;
    void buildCubaturePositionsAndWeights();
    void buildCubatureLagrange();
};

template <size_t N>
const Real Triangle<N>::sizeFactor = 1.0 / 2.0;

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Triangle.hpp"

// ========================================================================
#endif /* SEMBA_MATH_SIMPLEX_TRIANGLE_H_ */
