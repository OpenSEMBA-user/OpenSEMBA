#ifndef SIMPLEXLIN_H_
#define SIMPLEXLIN_H_

#include "Simplex.h"

namespace {
template <Int SIMPLIN_N>
class SimplexLin : public Simplex {
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
	SimplexLin();
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
	const FunctionPolynomial<Real>&
	 getLagr(const UInt i) const;
	const FunctionPolynomial<Real>&
	 getDLagr(const UInt i, const UInt f) const;
	void
	 printInfo() const;
private:
	// --- Rotation and extraction matrices ---------------------------
	StaMatrix<Int,np,np> P[faces];
	StaMatrix<Int,nfp,np> R[faces];
	CartesianVector<Int,nsc> nId[np];
	StaMatrix<Int,faces,nfp> sNId;
	// --- Lagrange polynomials ---------------------------------------
	FunctionPolynomial<Real> lagr[np];
	FunctionPolynomial<Real> dLagr[np][faces];
	// --- ca: Cubatured alpha, cda: cub. derived, cwaa: cub. weighted
	static const Real sizeFactor;
	static const UInt dimension = 1;
	CartesianVector<Int,nsc> cId[ncp];
	CartesianVector<Real,nsc> cPos[ncp];
	Real cw[ncp];
	Real ca[np][ncp];
	Real cda[np][faces][ncp];
	StaMatrix<Real,np,np> cwaa[ncp];
	StaMatrix<Real,np,np> cwada[ncp][faces];
	// =========== Methods ============================================
	StaMatrix<Int,SIMPLIN_NFP,SIMPLIN_NP>
	 RMatrix(const UInt s) const;
	DynMatrix<Int>
	 PMatrix(const UInt n, const UInt s) const;
	void
	 buildNodeIndices(
	  CartesianVector<Int,nsc> *res,
	  const UInt order,
	  const UInt nNodes) const;
	void
	 buildSideNodeIndices();
	UInt
	 numberOfNodes(const UInt order) const ;
	// --- Cubature build functions -----------------------------------
	void
	 buildCubaturePositionsAndWeights();
	void
	 buildCubatureLagrange();
};

template <Int SIMPLIN_N>
const Real SimplexLin<SIMPLIN_N>::sizeFactor = 1.0;
// ====================================================================
#include "SimplexLin.hpp"
}
// ====================================================================
#endif
