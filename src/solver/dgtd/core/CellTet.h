#ifndef CELLTET_H_
#define CELLTET_H_

#include "math/Simplex.h"
#include "math/MathMatrix.h"
#include "math/Polynomial.h"
#include "SmbData.h"
#include "physicalModel/GroupPhysicalModels.h"
#include "Cell.h"
#include <vector>
#include <set>

using namespace std;

template <Int TET_N>
class CellTet : public Cell {
#define TET_NP ((TET_N+1)*(TET_N+2)*(TET_N+3)/6)
#define TET_NFP ((TET_N+1)*(TET_N+2)/2)
public:
    static const SimplexTet<TET_N> tet;
    static const UInt np = TET_NP;
    static const UInt nfp = TET_NFP;
    static const UInt faces = 4;
    static const UInt vertices = 4;
    const Tet* base;
    UInt vmapP[faces][nfp]; // Node to Node of the contiguous element.
    CVecR3 n[np]; // Lagrange's base functions nodes pos.
    CellTet();
    virtual ~CellTet();
    bool isCurved() const;
    bool isCurvedFace(const UInt f) const;
    Real getVolume() const;
    Real getAreaOfFace(UInt face) const;
    bool isLocalSide(
            const UInt side,
            const SurfR* surf) const;
    bool isLocalSide(
            const SurfR* surf) const;
    UInt getFaces() const {return faces;}
    UInt getNbp() const {return base->numberOfCoordinates();}
    UInt getNbfp() const;
    UInt getNfp() const {return nfp;}
    UInt getNumberOfVertices() const {return vertices;}
    UInt getNodeVertex(const UInt i) const;
    UInt getId() const {return base->getId();}
    const CoordR3* getV(UInt i) const {return base->getV(i);}
    void getCMatrices(StaMatrix<Real,np,np> C[3]) const;
    CVecR3 getNode(const UInt i) const {return n[i];}
    const CoordR3*
    getSideBaseNode(
            const UInt f,
            const UInt i) const;
    const CoordR3* getSideVertexBaseNode(UInt f, UInt i) const;
    Cell*& getEtoEPointer(const Int) const;
    CVecR3 getSideNodePos(
            const UInt f,
            const UInt i) const;
    CVecR3 getSideNormal(
            const UInt f) const;
    virtual void getCurvedLIFTnormal(
            StaMatrix<Real,np,nfp> LIFTn[3],
            StaMatrix<Real,np,nfp> LIFTcn[3],
            StaMatrix<Real,np,nfp> LIFTrn[3],
            const UInt face) const;
    const Tet* getPtrToBase() const;
    UInt getSideNode(UInt f, UInt i) const;
    virtual StaMatrix<Real,TET_NP,TET_NP>
    getConductivityWithGeometricProfile(
            const PMVolumePML& mat,
            const UInt type,
            const Real maxSigma) const;
    virtual StaMatrix<Real,TET_NP,TET_NP> getMassMatrix() const;
    virtual void printInfo() const;
    void printMapsInfo() const;
protected:
    void init(
            const Tet* base_,
            const PMGroup* pMGroup);
    StaMatrix<Real,TET_NP,TET_NP> getCMatrix(
            const UInt x,
            const StaMatrix<Real,np,np>& invM,
            const StaMatrix<Real,4,3> cJHat[SimplexTet<1>::ncp],
            const StaMatrix<Real,4,4> cJ[SimplexTet<1>::ncp]) const;
    virtual StaMatrix<Real,TET_NP,TET_NP> getMassMatrix(
            const Real cJDet[SimplexTet<1>::ncp]) const;
    virtual StaMatrix<Real,TET_NP,TET_NP> getMassMatrixIntegratedWithScalar(
            const Real cScalar[SimplexTet<1>::ncp]) const;
    StaMatrix<Real,np,nfp> getLIFTMatrix(
            const UInt s,
            const StaMatrix<Real,np,nfp>& invM,
            const  Real csd[SimplexTri<1>::ncp]) const;
private:
    void  buildNodes();
};

template <Int TET_N>
const SimplexTet<TET_N> CellTet<TET_N>::tet;

template <Int TET_N>
class CellTet4 : public CellTet<TET_N> {
    friend class Tet4;
public:
    static const UInt np = TET_NP;
    static const UInt nfp = TET_NFP;
    static const UInt faces = 4;
    static const UInt vertices = 4;
    CellTet4();
    virtual ~CellTet4();
    CellTet4(const Tet* element, const PMGroup& pMGroup);
    bool isCurved() const;
    void printBCInfo() const;
    void printMapsInfo() const;
};

template <Int TET_N>
class CellTet10 : public CellTet<TET_N> {
public:
    static const UInt np = TET_NP;
    static const UInt nfp = TET_NFP;
    static const UInt faces = 4;
    static const UInt vertices = 4;
    //
    CellTet10();
    virtual ~CellTet10();
    CellTet10(const Tet* element, const PMGroup& pMGroup);
    void getCurvedLIFTnormal(
            StaMatrix<Real,np,nfp> LIFTn[3],
            StaMatrix<Real,np,nfp> LIFTcn[3],
            StaMatrix<Real,np,nfp> LIFTrn[3],
            const UInt face) const;
    void buildOperators();
};

#include "CellTet.hpp"

#endif /* CELL_H_ */
