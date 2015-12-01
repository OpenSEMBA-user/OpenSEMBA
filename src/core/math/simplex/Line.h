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

template <Int SIMPLIN_N>
class Line : public Simplex {
    #define SIMPLIN_NP (SIMPLIN_N+1)
    #define SIMPLIN_NFP (1)
    #define SIMPLIN_FACES (2)
public:
    static const UInt faces = 2;
    static const UInt nsc = 2;
    static const UInt n = SIMPLIN_N;
    static const UInt np = SIMPLIN_NP;
    static const UInt nfp = SIMPLIN_NFP;
    static const UInt nc = SIMPLEX_CUBATURE_ORDER;
    static const UInt ncp = SIMPLEX_CUBATURE_ORDER + 1;
    // ------- Methods ------------------------------------------------
    Line();
    UInt vertex(const UInt) const;
    UInt sideVertex(const UInt f, const UInt i) const;
    UInt nodeIndex(const UInt i, const UInt j) const;
    UInt cubatureNodeIndex(const UInt i, const UInt j) const;
    UInt sideNode(const UInt f, const UInt i) const;
    const Function::Polynomial<Real>& getLagr(const UInt i) const;
    const Function::Polynomial<Real>& getDLagr(const UInt i,
                                               const UInt f) const;
    void     printInfo() const;
private:
    // --- Rotation and extraction matrices ---------------------------
    Matrix::Static<Int,np,np> P[faces];
    Matrix::Static<Int,nfp,np> R[faces];
    Vector::Cartesian<Int,nsc> nId[np];
    Matrix::Static<Int,faces,nfp> sNId;
    // --- Lagrange polynomials ---------------------------------------
    Function::Polynomial<Real> lagr[np];
    Function::Polynomial<Real> dLagr[np][faces];
    // --- ca: Cubatured alpha, cda: cub. derived, cwaa: cub. weighted
    static const Real sizeFactor;
    static const UInt dimension = 1;
    Vector::Cartesian<Int,nsc> cId[ncp];
    Vector::Cartesian<Real,nsc> cPos[ncp];
    Real cw[ncp];
    Real ca[np][ncp];
    Real cda[np][faces][ncp];
    Matrix::Static<Real,np,np> cwaa[ncp];
    Matrix::Static<Real,np,np> cwada[ncp][faces];
    // =========== Methods ============================================
    Matrix::Static<Int,SIMPLIN_NFP,SIMPLIN_NP> RMatrix(const UInt s) const;
    Matrix::Dynamic<Int> PMatrix(const UInt n, const UInt s) const;
    void buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
                          const UInt order,
                          const UInt nNodes) const;
    void buildSideNodeIndices();
    UInt numberOfNodes(const UInt order) const ;
    // --- Cubature build functions -----------------------------------
    void buildCubaturePositionsAndWeights();
    void buildCubatureLagrange();
};

template <Int SIMPLIN_N>
const Real Line<SIMPLIN_N>::sizeFactor = 1.0;
// ====================================================================

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Line.hpp"
// ====================================================================
#endif /* SEMBA_MATH_SIMPLEX_LINE_H_ */
