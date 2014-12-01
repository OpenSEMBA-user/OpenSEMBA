#ifndef SIMPLEXLIN_H_
#define SIMPLEXLIN_H_

#include "Simplex.h"

namespace {
template <int SIMPLIN_N>
class SimplexLin : public Simplex {
#define SIMPLIN_NP (SIMPLIN_N+1)
#define SIMPLIN_NFP (1)
#define SIMPLIN_FACES (2)
public:
	static const unsigned int faces = 2;
	static const unsigned int nsc = 2;
	static const unsigned int n = SIMPLIN_N;
	static const unsigned int np = SIMPLIN_NP;
	static const unsigned int nfp = SIMPLIN_NFP;
	static const unsigned int nc = SIMPLEX_CUBATURE_ORDER;
	static const unsigned int ncp = SIMPLEX_CUBATURE_ORDER + 1;
	// ------- Methods ------------------------------------------------
	SimplexLin();
	unsigned int
	 vertex(const unsigned int) const;
	unsigned int
	 sideVertex(const unsigned int f, const unsigned int i) const;
	unsigned int
	 nodeIndex(const unsigned int i, const unsigned int j) const;
	unsigned int
	 cubatureNodeIndex(const unsigned int i, const unsigned int j) const;
	unsigned int
	 sideNode(const unsigned int f, const unsigned int i) const;
	const Polynomial<double>&
	 getLagr(const unsigned int i) const;
	const Polynomial<double>&
	 getDLagr(const unsigned int i, const unsigned int f) const;
	void
	 printInfo() const;
private:
	// --- Rotation and extraction matrices ---------------------------
	StaMatrix<int,np,np> P[faces];
	StaMatrix<int,nfp,np> R[faces];
	CartesianVector<int,nsc> nId[np];
	StaMatrix<int,faces,nfp> sNId;
	// --- Lagrange polynomials ---------------------------------------
	Polynomial<double> lagr[np];
	Polynomial<double> dLagr[np][faces];
	// --- ca: Cubatured alpha, cda: cub. derived, cwaa: cub. weighted
#ifndef __GXX_EXPERIMENTAL_CXX0X__
	static const double sizeFactor = 1.0;
#else
	static constexpr double sizeFactor = 1.0;
#endif
	static const unsigned int dimension = 1;
	CartesianVector<int,nsc> cId[ncp];
	CartesianVector<double,nsc> cPos[ncp];
	double cw[ncp];
	double ca[np][ncp];
	double cda[np][faces][ncp];
	StaMatrix<double,np,np> cwaa[ncp];
	StaMatrix<double,np,np> cwada[ncp][faces];
	// =========== Methods ============================================
	StaMatrix<int,SIMPLIN_NFP,SIMPLIN_NP>
	 RMatrix(const unsigned int s) const;
	DynMatrix<int>
	 PMatrix(const unsigned int n, const unsigned int s) const;
	void
	 buildNodeIndices(
	  CartesianVector<int,nsc> *res,
	  const unsigned int order,
	  const unsigned int nNodes) const;
	void
	 buildSideNodeIndices();
	unsigned int
	 numberOfNodes(const unsigned int order) const ;
	// --- Cubature build functions -----------------------------------
	void
	 buildCubaturePositionsAndWeights();
	void
	 buildCubatureLagrange();
};
// ====================================================================
#include "SimplexLin.cpp"
}
// ====================================================================
#endif
