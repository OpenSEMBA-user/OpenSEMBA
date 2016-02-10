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

#ifndef SEMBA_MATH_SIMPLEX_TETRAHEDRON_H_
#define SEMBA_MATH_SIMPLEX_TETRAHEDRON_H_

#include "Simplex.h"
#include "Triangle.h"

namespace SEMBA {
namespace Math {
namespace Simplex {

template <Int SIMPTET_N>
class Tetrahedron : public Simplex {
    #define SIMPTET_NP ((SIMPTET_N+1)*(SIMPTET_N+2)*(SIMPTET_N+3)/6)
    #define SIMPTET_NFP ((SIMPTET_N+1)*(SIMPTET_N+2)/2)
    #define SIMPTET_FACES (4)
public:
    static const std::size_t faces = 4;
    static const std::size_t nsc = 4;
    static const std::size_t n = SIMPTET_N;
    static const std::size_t np = SIMPTET_NP;
    static const std::size_t nfp = SIMPTET_NFP;
    const Triangle<SIMPTET_N> tri;
    Matrix::Static<Real,np,nfp> LIFT[faces];
    static const std::size_t nc = SIMPLEX_CUBATURE_ORDER;
    static const std::size_t ncp = (SIMPLEX_CUBATURE_ORDER + 1)*
                                   (SIMPLEX_CUBATURE_ORDER+2)*
                                   (SIMPLEX_CUBATURE_ORDER+3)/6;
    Real cw[ncp];
    Matrix::Static<Real,np,np> cwaa[ncp];
    Matrix::Static<Real,np,np> cwada[ncp][faces];
    // R: Extraction matrices.
    Matrix::Static<Int,nfp,np> R[faces];
    // ------- Methods --------------------------------------------------------
    Tetrahedron();
    Matrix::Static<Real,SIMPTET_NP,SIMPTET_NP> getMassMatrix() const;
    std::size_t vertex(const std::size_t) const;
    std::size_t sideVertex(const std::size_t f, const std::size_t i) const;
    std::size_t nodeIndex(const std::size_t i, const std::size_t j) const;
    std::size_t cubatureNodeIndex(const std::size_t i,
                                  const std::size_t j) const;
    std::size_t sideNode(const std::size_t f, const std::size_t i) const;
    const Function::Polynomial<Real>& getLagr(const std::size_t i) const;
    const Function::Polynomial<Real>& getDLagr(const std::size_t i, 
                                               const std::size_t f) const;
    Real getCda(std::size_t i, std::size_t j, std::size_t k) const;
    CVecR4 coordinate(const std::size_t i) const;
    CVecR4 sideCoordinate(const std::size_t f, const std::size_t i) const;
    Real integrateScalarsOnFace(const Real x[SIMPTET_NP],
                                const std::size_t f,
                                const Real area) const;
    Real integrateScalars(const Real x[SIMPTET_NP],
                          const Real volume) const;
    void printInfo() const;
    const Vector::Cartesian<Real, nsc> cubatureCoordinate(
            const std::size_t c) const {
        return cPos[c];
    }
private:
    Matrix::Static<Int,np,np> P[faces]; // P: Rotation matrices.
    // lagr: Non-zero Lagrange's pol. coeffs.
    Function::Polynomial<Real> lagr[np];
    // Non-zero Lagrange's pol. coeffs of derivatives. uint is NP, NFACES.
    Function::Polynomial<Real> dLagr[np][faces];
    // nId: List of node indices.
    Vector::Cartesian<Int,nsc> nId[np];
    // sNId: List of side nodes.
    Matrix::Static<Int,faces,nfp> sNId;
    Matrix::Static<Int,SIMPTET_NFP,SIMPTET_NP> RMatrix(
            const std::size_t s) const;
    Matrix::Dynamic<Int> PMatrix(const std::size_t n,
                                 const std::size_t s) const;
    void buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
                          const std::size_t order,
                          const std::size_t nNodes) const;
    void buildSideNodeIndices();
    static const Real sizeFactor;
    static const std::size_t dimension = 3;
    Vector::Cartesian<Int,nsc> cId[ncp];
    Vector::Cartesian<Real,nsc> cPos[ncp];
    Real ca[np][ncp];
    Real cda[np][faces][ncp];
    // --- Cubature build functions -------------------------------------------
    void buildCubaturePositionsAndWeights();
    void buildCubatureLagrange();
    void buildLIFTMatrix();
    Int numberOfNodes(Int order) const ;
};

template<Int SIMPTET_N>
const Real Tetrahedron<SIMPTET_N>::sizeFactor = 1.0 / 6.0;

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Tetrahedron.hpp"

// ========================================================================
#endif /* SEMBA_MATH_SIMPLEX_TETRAHEDRON_H_ */
