#ifndef SIMPLEXTRI_H_
#define SIMPLEXTRI_H_
// ----------- Compiler directives ----------------------------------------
// ----------- Libraries --------------------------------------------------
#include "Simplex.h"
#include "CartesianVector.h"
#include "Polynomial.h"
// =========== SimplexTri class ===========================================
namespace {
template <int SIMPTRI_N>
class SimplexTri : public Simplex {
	#define SIMPTRI_NP ((SIMPTRI_N+1)*(SIMPTRI_N+2)/2)
	#define SIMPTRI_NFP (SIMPTRI_N+1)
	#define SIMPTRI_FACES (3)
public:
	static const unsigned int faces = 3;
	static const unsigned int n = SIMPTRI_N;
	static const unsigned int nsc = 3;
	static const unsigned int vertices = 3;
	static const unsigned int np = SIMPTRI_NP;
	static const unsigned int nfp = SIMPTRI_NFP;
	static const unsigned int nc = SIMPLEX_CUBATURE_ORDER;
	static const unsigned int ncp =
	 (SIMPLEX_CUBATURE_ORDER+1)*(SIMPLEX_CUBATURE_ORDER+2)/2;
	double cw[ncp];
	CartesianVector<double,nsc> cPos[ncp];
	// Rotation and extraction matrices.
	StaMatrix<int,np,np> P[faces];
	StaMatrix<int,nfp,np> R[faces];
	CartesianVector<int,nsc> nId[np];
	StaMatrix<int,faces,nfp> sNId;
	Polynomial<double> lagr[np];
	Polynomial<double> dLagr[np][faces];
//	// weight: This is the relative weight of a node. Is computed
//	// integrating over the simplex area. The sum of all must be one.
//	// Weights can be used to obtain the result of a integral knowing the
//	// function values on its nodal positions.
//	double w[np];
	static const double sizeFactor;
	static const unsigned int dimension = 2;
	CartesianVector<int,nsc> cId[ncp];
	double ca[np][ncp];
	double cda[np][faces][ncp];
	StaMatrix<double,np,np> cwaa[ncp];
	StaMatrix<double,np,np> cwada[ncp][faces];
	SimplexTri();
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
	CartesianVector<double,3>
	 coordinate(const unsigned int i) const;
	const Polynomial<double>&
	 getLagr(const unsigned int i) const;
	const Polynomial<double>&
	 getDLagr(const unsigned int i, const unsigned int f) const;
	StaMatrix<double,SIMPTRI_NP,SIMPTRI_NP>
	 getMassMatrix() const;
	DynMatrix<int>
	 PMatrix(const unsigned int n, const unsigned int s) const;
	void
	 printInfo() const;
private:
	unsigned int
	 numberOfNodes(const unsigned int order) const;
	StaMatrix<int,SIMPTRI_NFP,SIMPTRI_NP>
	 RMatrix(const unsigned int s) const;
	void
	 buildNodeIndices(
	  CartesianVector<int,nsc> *res,
	  const unsigned int order,
	  const unsigned int nNodes) const;
	void
	 buildSideNodeIndices();
	int
	 numberOfNodes(int order) const ;
	void
	 buildCubaturePositionsAndWeights();
	void
	 buildCubatureLagrange();;
};

template <int SIMPTRI_N>
const double SimplexTri<SIMPTRI_N>::sizeFactor = 1.0 / 2.0;

#include "SimplexTri.hpp"
}
// ========================================================================
#endif
