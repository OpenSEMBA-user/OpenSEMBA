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

template <Int N, class E>
class CellTet : public Cell, public E {
public:
    static const SimplexTet<N> tet;
    static const UInt np = ((N+1)*(N+2)*(N+3)/6);
    static const UInt nfp = ((N+1)*(N+2)/2);
    typedef StaMatrix<Real,np,np> MatRNpNp;
    CellTet(const E& base);
    virtual ~CellTet();
    bool isLocalSide(
            const UInt side,
            const SurfR* surf) const;
    bool isLocalSide(
            const SurfR* surf) const;
    UInt getNfp() const {return nfp;}
    UInt getNodeVertex(const UInt i) const;
    UInt getId() const {return base->getId();}
    const CoordR3* getV(UInt i) const {return base->getV(i);}
    void getCMatrices(StaMatrix<Real,np,np> C[3]) const;
    CVecR3 getNode(const UInt i) const {return n[i];}
    Cell*& getEtoEPointer(const Int) const;
    CVecR3 getSideNodePos(
            const UInt f,
            const UInt i) const;
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
    void getCurvedLIFTnormal(
            StaMatrix<Real,np,nfp> LIFTn[3],
            StaMatrix<Real,np,nfp> LIFTcn[3],
            StaMatrix<Real,np,nfp> LIFTrn[3],
            const UInt face) const;
    void buildOperators();
protected:
    void init(
            const Tet* base_,
            const PMGroup* pMGroup);
    MatRNpNp getCMatrix(
            const UInt x,
            const MatRNpNp& invM,
            const MatR43 cJHat[SimplexTet<1>::ncp],
            const MatR44 cJ[SimplexTet<1>::ncp]) const;
    virtual StaMatrix<Real,TET_NP,TET_NP> getMassMatrix(
            const Real cJDet[SimplexTet<1>::ncp]) const;
    virtual StaMatrix<Real,TET_NP,TET_NP> getMassMatrixIntegratedWithScalar(
            const Real cScalar[SimplexTet<1>::ncp]) const;
    StaMatrix<Real,np,nfp> getLIFTMatrix(
            const UInt s,
            const StaMatrix<Real,np,nfp>& invM,
            const  Real csd[SimplexTri<1>::ncp]) const;
private:
    array<CVecR3,np> nodes_;
    void  buildNodes();
};

template <Int N>
const SimplexTet<N> CellTet<N>::tet;

#include "CellTet.hpp"

#endif /* CELL_H_ */
