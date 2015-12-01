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
    static const UInt faces = 4;
    static const UInt nsc = 4;
    static const UInt n = SIMPTET_N;
    static const UInt np = SIMPTET_NP;
    static const UInt nfp = SIMPTET_NFP;
    const Triangle<SIMPTET_N> tri;
    Matrix::Static<Real,np,nfp> LIFT[faces];
    static const UInt nc = SIMPLEX_CUBATURE_ORDER;
    static const UInt ncp = (SIMPLEX_CUBATURE_ORDER+1)*
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
    UInt vertex(const UInt) const;
    UInt sideVertex(const UInt f, const UInt i) const;
    UInt nodeIndex(const UInt i, const UInt j) const;
    UInt cubatureNodeIndex(const UInt i, const UInt j) const;
    UInt sideNode(const UInt f, const UInt i) const;
    const Function::Polynomial<Real>& getLagr(const UInt i) const;
    const Function::Polynomial<Real>& getDLagr(const UInt i, const UInt f) const;
    Real getCda(UInt i, UInt j, UInt k) const;
    CVecR4 coordinate(const UInt i) const;
    CVecR4 sideCoordinate(const UInt f, const UInt i) const;
    Real integrateScalarsOnFace(const Real x[SIMPTET_NP],
                                const UInt f,
                                const Real area) const;
    Real integrateScalars(const Real x[SIMPTET_NP],
                          const Real volume) const;
    void printInfo() const;
    const Vector::Cartesian<Real, nsc> cubatureCoordinate(const UInt c) const {
        return cPos[c];
    }
private:
    Matrix::Static<Int,np,np> P[faces]; // P: Rotation matrices.
    // lagr: Non-zero Lagrange's pol. coeffs.
    Function::Polynomial<Real> lagr[np];
    // Non-zero Lagrange's pol. coeffs of derivatives. Size is NP, NFACES.
    Function::Polynomial<Real> dLagr[np][faces];
    // nId: List of node indices.
    Vector::Cartesian<Int,nsc> nId[np];
    // sNId: List of side nodes.
    Matrix::Static<Int,faces,nfp> sNId;
    Matrix::Static<Int,SIMPTET_NFP,SIMPTET_NP> RMatrix(const UInt s) const;
    Matrix::Dynamic<Int> PMatrix(const UInt n, const UInt s) const;
    void buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
                          const UInt order,
                          const UInt nNodes) const;
    void buildSideNodeIndices();
    static const Real sizeFactor;
    static const UInt dimension = 3;
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
