#ifndef SIMPLEXTET_H_
#define SIMPLEXTET_H_
// ----------- Libraries ------------------------------------------------------
#include "Simplex.h"
#include "SimplexTri.h"
// =========== SimplexTet class ===============================================
namespace {
template <int SIMPTET_N>
class SimplexTet : public Simplex {
#define SIMPTET_NP ((SIMPTET_N+1)*(SIMPTET_N+2)*(SIMPTET_N+3)/6)
#define SIMPTET_NFP ((SIMPTET_N+1)*(SIMPTET_N+2)/2)
#define SIMPTET_FACES (4)
public:
	static const unsigned int faces = 4;
	static const unsigned int nsc = 4;
	static const unsigned int n = SIMPTET_N;
	static const unsigned int np = SIMPTET_NP;
	static const unsigned int nfp = SIMPTET_NFP;
	const SimplexTri<SIMPTET_N> tri;
	StaMatrix<double,np,nfp> LIFT[faces];
	static const unsigned int nc = SIMPLEX_CUBATURE_ORDER;
	static const unsigned int ncp =
	 (SIMPLEX_CUBATURE_ORDER+1)*(SIMPLEX_CUBATURE_ORDER+2)
	  *(SIMPLEX_CUBATURE_ORDER+3)/6;
	double cw[ncp];
	StaMatrix<double,np,np> cwaa[ncp];
	StaMatrix<double,np,np> cwada[ncp][faces];
	// R: Extraction matrices.
	StaMatrix<int,nfp,np> R[faces];
	// ------- Methods --------------------------------------------------------
	SimplexTet();
	StaMatrix<double,SIMPTET_NP,SIMPTET_NP>
	 getMassMatrix() const;
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
	double
	 getCda(unsigned int i, unsigned int j, unsigned int k) const;
	CartesianVector<double,4>
	 coordinate(const unsigned int i) const;
	CartesianVector<double,4>
	 sideCoordinate(const unsigned int f, const unsigned int i) const;
	double
	 integrateScalarsOnFace(
	  const double x[SIMPTET_NP],
	  const unsigned int f,
	  const double area) const;
	double
	 integrateScalars(
	  const double x[SIMPTET_NP],
	  const double volume) const;
	void
	 printInfo() const;
	const CartesianVector<double, nsc>
	 cubatureCoordinate(const unsigned int c) const {
		 return cPos[c];
	 }
private:
	// P: Rotation matrices.
	StaMatrix<int,np,np> P[faces];
	// lagr: Non-zero Lagrange's pol. coeffs.
	Polynomial<double> lagr[np];
	// Non-zero Lagrange's pol. coeffs of derivatives. Size is NP, NFACES.
	Polynomial<double> dLagr[np][faces];
	// nId: List of node indices.
	CartesianVector<int,nsc> nId[np];
	// sNId: List of side nodes.
	StaMatrix<int,faces,nfp> sNId;
	StaMatrix<int,SIMPTET_NFP,SIMPTET_NP>
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
#ifdef __GXX_EXPERIMENTAL_CXX0X__
	static constexpr double sizeFactor = 1.0 / 6.0;
#else
	static const double sizeFactor = 1.0 / 6.0;
#endif
	static const unsigned int dimension = 3;
	CartesianVector<int,nsc> cId[ncp];
	CartesianVector<double,nsc> cPos[ncp];
	double ca[np][ncp];
	double cda[np][faces][ncp];
	// --- Cubature build functions -------------------------------------------
	void
	 buildCubaturePositionsAndWeights();
	void
	 buildCubatureLagrange();
	void
	 buildLIFTMatrix();
	int
	 numberOfNodes(int order) const ;
};
// ========================================================================
#include "SimplexTet.hpp"
}
// ========================================================================
#endif
