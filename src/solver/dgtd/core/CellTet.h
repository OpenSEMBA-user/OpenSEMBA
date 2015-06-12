#ifndef CELLTET_H_
#define CELLTET_H_

#include "../../../common/math/Simplex.h"
#include "../../../common/math/MathMatrix.h"
#include "../../../common/math/Polynomial.h"
#include "../../../common/SmbData.h"
#include "Cell.h"
#include <vector>
#include <set>

using namespace std;

#define CELL_NODE_TOLERANCE 1e-12
#ifndef CELL_ERROR
#	define CELL_ERROR 22
#endif

template <int TET_N>
class CellTet : public Cell {
#define TET_NP ((TET_N+1)*(TET_N+2)*(TET_N+3)/6)
#define TET_NFP ((TET_N+1)*(TET_N+2)/2)
public:
	static const SimplexTet<TET_N> tet;
	static const uint np = TET_NP;
	static const uint nfp = TET_NFP;
	static const uint faces = 4;
	static const uint vertices = 4;
	const Tet* base;
	uint vmapP[faces][nfp]; // Node to Node of the contiguous element.
	CVecR3 n[np]; // Lagrange's base functions nodes pos.
	CellTet();
	virtual ~CellTet();
	bool
	 isCurved() const;
	bool
	 isCurvedFace(const uint f) const;
	double
	 getVolume() const;
	double
	 getAreaOfFace(uint face) const;
	bool
	 isLocalSide(
	  const uint side,
	  const Surface* surf) const;
	bool
	 isLocalSide(
	  const Surface* surf) const;
	uint
	 getFaces() const {return faces;}
	uint
	 getNbp() const {return base->numberOfCoordinates();}
	uint
	 getNbfp() const;
	uint
	 getNfp() const {return nfp;}
	uint
	 getNumberOfVertices() const {return vertices;}
	uint
	 getNodeVertex(const uint i) const;
	uint
	 getId() const {return base->getId();}
	const CoordD3*
	 getV(uint i) const {return base->getV(i);}
	void
	 getCMatrices(
	  StaMatrix<double,np,np> C[3]) const;
	CartesianVector<double, 3>
	 getNode(const uint i) const {return n[i];}
	const CoordD3*
	 getSideBaseNode(
	  const uint f,
	  const uint i) const;
	const CoordD3*
	 getSideVertexBaseNode(uint f, uint i) const;
	Cell*&
	 getEtoEPointer(const int) const;
	CVecR3
	 getSideNodePos(
	  const uint f,
	  const uint i) const;
	CVecR3
	 getSideNormal(
	  const uint f) const;
	virtual void
	 getCurvedLIFTnormal(
	  StaMatrix<double,np,nfp> LIFTn[3],
	  StaMatrix<double,np,nfp> LIFTcn[3],
	  StaMatrix<double,np,nfp> LIFTrn[3],
	  const uint face) const;
	const Tet*
	 getPtrToBase() const;
	uint
	 getSideNode(uint f, uint i) const;
	virtual StaMatrix<double,TET_NP,TET_NP>
	 getConductivityWithGeometricProfile(
	  const PMVolumePML& mat,
	  const uint type,
	  const double maxSigma) const;
	virtual StaMatrix<double,TET_NP,TET_NP>
	 getMassMatrix() const;
	virtual void
	 printInfo() const;
	void
	 printMapsInfo() const;
protected:
	void
	 init(
	  const Tet* base_,
	  const PhysicalModelGroup* pMGroup);
	StaMatrix<double,TET_NP,TET_NP>
	 getCMatrix(
	  const uint x,
	  const StaMatrix<double,np,np>& invM,
	  const StaMatrix<double,4,3> cJHat[SimplexTet<1>::ncp],
	  const StaMatrix<double,4,4> cJ[SimplexTet<1>::ncp]) const;
	virtual StaMatrix<double,TET_NP,TET_NP>
	 getMassMatrix(
	  const double cJDet[SimplexTet<1>::ncp]) const;
	virtual StaMatrix<double,TET_NP,TET_NP>
	 getMassMatrixIntegratedWithScalar(
	  const double cScalar[SimplexTet<1>::ncp]) const;
	StaMatrix<double,np,nfp>
	 getLIFTMatrix(
	  const uint s,
	  const StaMatrix<double,np,nfp>& invM,
	  const  double csd[SimplexTri<1>::ncp]) const;
private:
	void
	 buildNodes();
};

template <int TET_N>
const SimplexTet<TET_N> CellTet<TET_N>::tet;

template <int TET_N>
class CellTet4 : public CellTet<TET_N> {
	friend class Tet4;
public:
	static const uint np = TET_NP;
	static const uint nfp = TET_NFP;
	static const uint faces = 4;
	static const uint vertices = 4;
	CellTet4();
	virtual ~CellTet4();
	CellTet4(
	 const Tet* element,
	 const PhysicalModelGroup& pMGroup);
	bool
	 isCurved() const;
	void
	 printBCInfo() const;
	void
	 printMapsInfo() const;
};

template <int TET_N>
class CellTet10 : public CellTet<TET_N> {
public:
	static const uint np = TET_NP;
	static const uint nfp = TET_NFP;
	static const uint faces = 4;
	static const uint vertices = 4;
	//
	CellTet10();
	virtual ~CellTet10();
	CellTet10(
	 const Tet* element,
	 const PhysicalModelGroup& pMGroup);
	void
	 getCurvedLIFTnormal(
	  StaMatrix<double,np,nfp> LIFTn[3],
	  StaMatrix<double,np,nfp> LIFTcn[3],
	  StaMatrix<double,np,nfp> LIFTrn[3],
	  const uint face) const;
	void
	 buildOperators();
};

#include "CellTet.hpp"
#endif /* CELL_H_ */
