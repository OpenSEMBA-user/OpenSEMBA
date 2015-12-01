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
#ifndef SIMPLEXTRI_H_
#define SIMPLEXTRI_H_
// ----------- Compiler directives ----------------------------------------
// ----------- Libraries --------------------------------------------------
#include "Simplex.h"
#include "CartesianVector.h"
#include "FunctionPolynomial.h"
// =========== SimplexTri class ===========================================
namespace {
template <Int SIMPTRI_N>
class SimplexTri : public Simplex {
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
	CartesianVector<Real,nsc> cPos[ncp];
	// Rotation and extraction matrices.
	StaMatrix<Int,np,np> P[faces];
	StaMatrix<Int,nfp,np> R[faces];
	CartesianVector<Int,nsc> nId[np];
	StaMatrix<Int,faces,nfp> sNId;
	FunctionPolynomial<Real> lagr[np];
	FunctionPolynomial<Real> dLagr[np][faces];
//	// weight: This is the relative weight of a node. Is computed
//	// integrating over the simplex area. The sum of all must be one.
//	// Weights can be used to obtain the result of a integral knowing the
//	// function values on its nodal positions.
//	Real w[np];
	static const Real sizeFactor;
	static const UInt dimension = 2;
	CartesianVector<Int,nsc> cId[ncp];
	Real ca[np][ncp];
	Real cda[np][faces][ncp];
	StaMatrix<Real,np,np> cwaa[ncp];
	StaMatrix<Real,np,np> cwada[ncp][faces];
	SimplexTri();
	UInt
	 vertex(const UInt) const;
	UInt
	 sideVertex(const UInt f, const UInt i) const;
	UInt
	 nodeIndex(const UInt i, const UInt j) const;
	UInt
	 cubatureNodeIndex(const UInt i, const UInt j) const;
	UInt
	 sideNode(const UInt f, const UInt i) const;
	CartesianVector<Real,3>
	 coordinate(const UInt i) const;
	const FunctionPolynomial<Real>&
	 getLagr(const UInt i) const;
	const FunctionPolynomial<Real>&
	 getDLagr(const UInt i, const UInt f) const;
	StaMatrix<Real,SIMPTRI_NP,SIMPTRI_NP>
	 getMassMatrix() const;
	DynMatrix<Int>
	 PMatrix(const UInt n, const UInt s) const;
	void
	 printInfo() const;
private:
	UInt
	 numberOfNodes(const UInt order) const;
	StaMatrix<Int,SIMPTRI_NFP,SIMPTRI_NP>
	 RMatrix(const UInt s) const;
	void
	 buildNodeIndices(
	  CartesianVector<Int,nsc> *res,
	  const UInt order,
	  const UInt nNodes) const;
	void
	 buildSideNodeIndices();
	Int
	 numberOfNodes(Int order) const ;
	void
	 buildCubaturePositionsAndWeights();
	void
	 buildCubatureLagrange();;
};

template <Int SIMPTRI_N>
const Real SimplexTri<SIMPTRI_N>::sizeFactor = 1.0 / 2.0;

#include "SimplexTri.hpp"
}
// ========================================================================
#endif
