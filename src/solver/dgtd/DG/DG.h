#ifndef DG_H_
#define DG_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "../core/Comm.h"
#include "SmbData.h"
#include "options/OptionsSolverDGTD.h"
#include "math/Field.h"
#include "sources/DGPlaneWave.h"
#include "sources/DGDipole.h"
#include "sources/DGWaveportRectangular.h"
#include "dispersives/DGSIBC.h"
#include "dispersives/DGDispersiveVolumic.h"
#include "dispersives/DGPMLUniaxial.h"
#include "dispersives/DGPMLMultiaxial.h"

#define SOLVER_DEDUPLICATE_OPERATORS
#define SOLVER_USE_OPENMP

struct lexCompareMat {
    static const UInt np = ((ORDER_N+1) * (ORDER_N+2) * (ORDER_N+3) / 6);
    bool
    operator() (
            const StaMatrix<Real,np,np>& lhs,
            const StaMatrix<Real,np,np>& rhs) const {
        static const Real tolerance = 1e-12;
        for (UInt i = 0; i < (np*np); i++) {
            if (abs(lhs.val(i) - rhs.val(i)) > tolerance) {
                if (lhs.val(i) < rhs.val(i)) {
                    return true;
                }
                if (lhs.val(i) > rhs.val(i)) {
                    return false;
                }
            }
        }
        return false;
    }
};

class DG : public Ordering {
    friend class Exporter;
    friend class IntegratorLSERK;
    friend class IntegratorLF2;
    friend class IntegratorLF2Full;
    friend class IntegratorVerlet;
public:
    const static UInt N = ORDER_N;
    const static UInt faces = 4;
    const static UInt np = (N+1) * (N+2) * (N+3) / 6;
    const static UInt np2 = np * 2;
    const static UInt nfp = (N+1) * (N+2) / 2;
    const static UInt npnfp = np * nfp;
    const static UInt npnp = np * np;
    const static UInt nfpfaces = nfp * faces;
    DG();
    virtual ~DG();
    virtual void setFieldsToZero();
    virtual void setFieldsToOne();
    virtual void setFieldsToRandom();
    virtual void setFieldsToGaussian(
            const Real amplitude,
            CVecR3& polarization,
            const CVecR3& gaussCenter,
            const Real gaussWidth);
    virtual void setFieldsToHarmonics(
            const CartesianVector<Int,3>& harmonics,
            CVecR3& polarization);
    void setFieldsAndTimeFromResumeFile();
    virtual UInt getFieldDOFs() = 0;
    const FieldR3* getElectric() const;
    const FieldR3* getMagnetic() const;
    virtual UInt getGlobalFieldPosOfVertex(
            pair<const ElemR*, UInt> vertex) const;
    virtual vector<UInt> getGlobalFieldPosOfFace(
            Face boundary) const;
    virtual vector<UInt> getGlobalFieldPosOfVolume(
            const UInt volId) const;
    virtual void printInfo() const = 0;

protected:
    FieldR3 E, H;
    FieldR3 resE, resH;
    FieldR3 nAdm, nImp, rnAdm, rnImp, cnAdm, cnImp;
    Real *oneOverEps, *oneOverMu;
    Comm* comm;
    const SmbData* smb_;
    // Flux gatherer operator. dim = matrix(np x (4*nfp))
    Real LIFT[faces*npnfp];
#ifdef SOLVER_DEDUPLICATE_OPERATORS
    set<StaMatrix<Real,np,np>, lexCompareMat> CList;
#else
    StaMatrix<Real,np,np>* CList;
#endif
    void init(
            const OptionsSolverDGTD* arg,
            const PMGroup* pm_,
            Comm* comm_);
    virtual void addFluxesToRHS(
            const UInt e1, const UInt e2,
            const Real localTime,
            const Real minDT);
    virtual void addFluxesToRHSElectric(const UInt e1, const UInt e2) = 0;
    virtual void addFluxesToRHSMagnetic(const UInt e1, const UInt e2) = 0;
    virtual void addFluxesToRHSElectric(
            const UInt e1, const UInt e2, const bool useResForUpw) = 0;
    virtual void addFluxesToRHSMagnetic(
            const UInt e1, const UInt e2, const bool useResForUpw) = 0;
    virtual void addRHSToFieldsElectric(
            const UInt e1,
            const UInt e2,
            const Real rkdt) = 0;
    virtual void addRHSToFieldsMagnetic(
            const UInt e1,
            const UInt e2,
            const Real rkdt) = 0;
    virtual void addRHSToResidueElectric(
            const UInt e1, const UInt e2, const Real rkdt) = 0;
    virtual void addRHSToResidueMagnetic(
            const UInt e1, const UInt e2, const Real rkdt) = 0;
    void buildCMatrices();
    virtual void buildMaterials(
            const OptionsSolverDGTD* arg) = 0;
    virtual void  computeRHS(
            const UInt e1,
            const UInt e2,
            const Real localtime,
            const Real rkdt) = 0;
    virtual void computeRHSElectric(
            const UInt e1,
            const UInt e2,
            const Real localtime,
            const Real minDT) = 0;
    virtual void computeRHSMagnetic(
            const UInt e1,
            const UInt e2,
            const Real localtime,
            const Real minDT) = 0;
    virtual UInt getIndexOfElement(const UInt e) const = 0;
    virtual const FieldR3& getRHSElectric() const = 0;
    virtual const FieldR3& getRHSMagnetic() const = 0;
    virtual void updateFieldsWithRes(
            const UInt e1,
            const UInt e2,
            const Real rkb) = 0;
    virtual void updateFieldsWithResBase(
            const UInt e1,
            const UInt e2,
            const Real rkb);
    virtual void computeCurlsInRHS(const UInt e1, const UInt e2);
    virtual void computeCurlsInRHSElectric(const UInt e1, const UInt e2) = 0;
    virtual void computeCurlsInRHSMagnetic(const UInt e1, const UInt e2) = 0;
    virtual void computeJumps(
            const UInt e1,
            const UInt e2,
            const Real localTime,
            const Real minDT) = 0;
    void copyFieldsInResidues(
            const UInt e1,
            const UInt e2);
    virtual void copyJumpsToResidueJumps(
            const UInt e1,
            const UInt e2) = 0;
    virtual void computePolarizationCurrentsRHS(
            const UInt e1, const UInt e2) = 0;
    virtual void computePolarizationCurrentsRHSElectric(
            const UInt e1, const UInt e2) = 0;
    virtual void computePolarizationCurrentsRHSMagnetic(
            const UInt e1, const UInt e2) = 0;
    virtual void LTSSaveFieldsAndResidues(
            const UInt fKSave,
            const UInt lKSave) = 0;
    virtual void LTSLoadFieldsAndResidues(
            const UInt fKSave,
            const UInt lKSave) = 0;
    virtual void addRHSToRes(
            const UInt e1,
            const UInt e2,
            const Real rka,
            const Real dt) = 0;
    void swapResiduesAndFields(
            const UInt e1,
            const UInt e2);
    void buildFluxScalingFactors(const MapGroup& map);
    void buildFieldScalingFactors();
private:
    virtual void buildScalingFactors(const MapGroup& map);
    void buildLIFT();
    virtual void assignMatrices() = 0;
    void allocateFieldsAndRes();
    void setResidualsToZero();
};
#endif /* SOLVER_H_ */

