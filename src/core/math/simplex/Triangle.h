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

// =========== Triangle class ===========================================
namespace SEMBA {
namespace Math {
namespace Simplex {

template <Int SIMPTRI_N>
class Triangle : public Simplex {
    #define SIMPTRI_NP ((SIMPTRI_N+1)*(SIMPTRI_N+2)/2)
    #define SIMPTRI_NFP (SIMPTRI_N+1)
    #define SIMPTRI_FACES (3)
public:
    static const UInt faces = 3;
    static const UInt n = SIMPTRI_N;
    static const UInt nsc = 3;
    static const UInt vertices = 3;
    static const UInt np = SIMPTRI_NP;
    static const UInt nfp = SIMPTRI_NFP;
    static const UInt nc = SIMPLEX_CUBATURE_ORDER;
    static const UInt ncp =
    (SIMPLEX_CUBATURE_ORDER+1)*(SIMPLEX_CUBATURE_ORDER+2)/2;
    Real cw[ncp];
    Vector::Cartesian<Real,nsc> cPos[ncp];
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
    static const UInt dimension = 2;
    Vector::Cartesian<Int,nsc> cId[ncp];
    Real ca[np][ncp];
    Real cda[np][faces][ncp];
    Matrix::Static<Real,np,np> cwaa[ncp];
    Matrix::Static<Real,np,np> cwada[ncp][faces];
    Triangle();
    UInt vertex(const UInt) const;
    UInt sideVertex(const UInt f, const UInt i) const;
    UInt nodeIndex(const UInt i, const UInt j) const;
    UInt cubatureNodeIndex(const UInt i, const UInt j) const;
    UInt sideNode(const UInt f, const UInt i) const;
    Vector::Cartesian<Real,3> coordinate(const UInt i) const;
    const Function::Polynomial<Real>& getLagr(const UInt i) const;
    const Function::Polynomial<Real>& getDLagr(const UInt i,
                                               const UInt f) const;
    Matrix::Static<Real,SIMPTRI_NP,SIMPTRI_NP> getMassMatrix() const;
    Matrix::Dynamic<Int> PMatrix(const UInt n, const UInt s) const;
    void printInfo() const;
private:
    UInt numberOfNodes(const UInt order) const;
    Matrix::Static<Int,SIMPTRI_NFP,SIMPTRI_NP> RMatrix(const UInt s) const;
    void buildNodeIndices(Vector::Cartesian<Int,nsc> *res,
                          const UInt order,
                          const UInt nNodes) const;
    void buildSideNodeIndices();
    Int numberOfNodes(Int order) const ;
    void buildCubaturePositionsAndWeights();
    void buildCubatureLagrange();
};

template <Int SIMPTRI_N>
const Real Triangle<SIMPTRI_N>::sizeFactor = 1.0 / 2.0;

} /* namespace Simplex */
} /* namespace Math */
} /* namespace SEMBA */

#include "Triangle.hpp"

// ========================================================================
#endif /* SEMBA_MATH_SIMPLEX_TRIANGLE_H_ */
