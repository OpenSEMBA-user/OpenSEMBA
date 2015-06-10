#ifndef DG_H_
#define DG_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../core/ArgumentsCudg3d.h"
#include "../../../common/math/Field.h"
#include "../core/Comm.h"
#include "sources/DGPlaneWave.h"
#include "sources/DGDipole.h"
#include "sources/DGWaveportRectangular.h"
#include "VectorModuleResult.h"
#include "../core/BCGroup.h"

#include "dispersives/DGSIBC.h"
#include "dispersives/DGDispersiveVolumic.h"
#include "dispersives/DGPMLUniaxial.h"
#include "dispersives/DGPMLMultiaxial.h"

using namespace std;

#define SOLVER_DEDUPLICATE_OPERATORS
#define SOLVER_USE_OPENMP

#ifdef SOLVER_DEDUPLICATE_OPERATORS
struct lexCompareMat {
   static const uint np = ((ORDER_N+1) * (ORDER_N+2) * (ORDER_N+3) / 6);
   bool
   operator() (
         const StaMatrix<double,np,np>& lhs,
         const StaMatrix<double,np,np>& rhs) const {
      static const double tolerance = 1e-12;
      for (uint i = 0; i < (np*np); i++) {
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
#endif

class DG : public Ordering {
   friend class Exporter;
   friend class IntegratorLSERK;
   friend class IntegratorLF2;
   friend class IntegratorLF2Full;
   friend class IntegratorVerlet;
public:
   const static uint faces = 4;
   const static uint N = ORDER_N;
   const static uint np = (N+1) * (N+2) * (N+3) / 6;
   const static uint np2 = np * 2;
   const static uint nfp = (N+1) * (N+2) / 2;
   const static uint npnfp = np * nfp;
   const static uint npnp = np * np;
   const static uint nfpfaces = nfp * faces;
   DG();
   virtual ~DG();
   virtual void
   setFieldsToZero();
   virtual void
   setFieldsToOne();
   virtual void
   setFieldsToRandom();
   virtual void
   setFieldsToGaussian(
         const CellGroup& cells,
         const double amplitude,
         CVecD3& polarization,
         const CVecD3& gaussCenter,
         const double gaussWidth);
   virtual void
   setFieldsToHarmonics(
         const CellGroup& cells,
         const CartesianVector<int,3>& harmonics,
         CVecD3& polarization);
   void
   setFieldsAndTimeFromResumeFile();
   virtual uint
   getFieldDOFs() = 0;
   const FieldD3* getElectric() const;
   const FieldD3* getMagnetic() const;
   virtual uint getGlobalFieldPosOfVertex(
         pair<const Element*, uint> vertex) const;
   virtual vector<uint> getGlobalFieldPosOfFace(
         pair<const Volume*, uint> boundary) const;
   virtual vector<uint> getGlobalFieldPosOfVolume(
         const uint volId) const;
   virtual void printInfo() const = 0;

protected:
   // Fields and residuals: dim = (np,nK)
   FieldD3 E, H;
   FieldD3 resE, resH;
   // Scaling factors for linear elements.
   // Fluxes scaling factors. dim = (4, nK)
   FieldD3 nAdm, nImp, rnAdm, rnImp, cnAdm, cnImp;
   // Field scaling factors. dim = (nK)
   double *oneOverEps, *oneOverMu;
   uint nK;
   double upwinding;
   Comm* comm;
   const SmbData* smb_;
   // Flux gatherer operator. dim = matrix(np x (4*nfp))
   double LIFT[faces*npnfp];
#ifdef SOLVER_DEDUPLICATE_OPERATORS
   set<StaMatrix<double,np,np>, lexCompareMat> CList;
#else
   StaMatrix<double,np,np>* CList;
#endif
   void
   init(
         const OptionsSolverDGTD* arg,
         const PhysicalModelGroup* pm_,
         const CellGroup* cells, Comm* comm_);
   virtual void
   addFluxesToRHS(
         const uint e1, const uint e2,
         const double localTime,
         const double minDT);
   virtual void
   addFluxesToRHSElectric(const uint e1, const uint e2) = 0;
   virtual void
   addFluxesToRHSMagnetic(const uint e1, const uint e2) = 0;
   virtual void
   addFluxesToRHSElectric(
         const uint e1, const uint e2, const bool useResForUpw) = 0;
   virtual void
   addFluxesToRHSMagnetic(
         const uint e1, const uint e2, const bool useResForUpw) = 0;

   virtual void
   addRHSToFieldsElectric(
         const uint e1,
         const uint e2,
         const double rkdt) = 0;
   virtual void
   addRHSToFieldsMagnetic(
         const uint e1,
         const uint e2,
         const double rkdt) = 0;

   virtual void
   addRHSToResidueElectric(
         const uint e1, const uint e2, const double rkdt) = 0;
   virtual void
   addRHSToResidueMagnetic(
         const uint e1, const uint e2, const double rkdt) = 0;

   void
   buildCMatrices(const CellGroup& cell);
   virtual void
   buildMaterials(
         const CellGroup& cells,
         const OptionsSolverDGTD* arg) = 0;
   virtual void
   computeRHS(
         const uint e1,
         const uint e2,
         const double localtime,
         const double rkdt) = 0;
   virtual void
   computeRHSElectric(
         const uint e1,
         const uint e2,
         const double localtime,
         const double minDT) = 0;
   virtual void
   computeRHSMagnetic(
         const uint e1,
         const uint e2,
         const double localtime,
         const double minDT) = 0;
   virtual uint
   getIndexOfElement(const uint e) const = 0;
   virtual const Field<double,3>&
   getRHSElectric() const = 0;
   virtual const Field<double,3>&
   getRHSMagnetic() const = 0;
   virtual void
   updateFieldsWithRes(
         const uint e1,
         const uint e2,
         const double rkb) = 0;
   virtual void
   updateFieldsWithResBase(
         const uint e1,
         const uint e2,
         const double rkb);
   virtual void
   computeCurlsInRHS(const uint e1, const uint e2);
   virtual void
   computeCurlsInRHSElectric(const uint e1, const uint e2) = 0;
   virtual void
   computeCurlsInRHSMagnetic(const uint e1, const uint e2) = 0;
   virtual void
   computeJumps(
         const uint e1,
         const uint e2,
         const double localTime,
         const double minDT) = 0;
   void
   copyFieldsInResidues(
         const uint e1,
         const uint e2);
   virtual void
   copyJumpsToResidueJumps(
         const uint e1,
         const uint e2) = 0;
   virtual void
   computePolarizationCurrentsRHS(
         const uint e1, const uint e2) = 0;
   virtual void
   computePolarizationCurrentsRHSElectric(
         const uint e1, const uint e2) = 0;
   virtual void
   computePolarizationCurrentsRHSMagnetic(
         const uint e1, const uint e2) = 0;
   virtual void
   LTSSaveFieldsAndResidues(
         const uint fKSave,
         const uint lKSave) = 0;
   virtual void
   LTSLoadFieldsAndResidues(
         const uint fKSave,
         const uint lKSave) = 0;
   virtual void
   addRHSToRes(
         const uint e1,
         const uint e2,
         const double rka,
         const double dt) = 0;
   void
   swapResiduesAndFields(
         const uint e1,
         const uint e2);
   void
   buildFluxScalingFactors(
         const CellGroup& cells,
         const MapGroup& map);
   void
   buildFieldScalingFactors(
         const CellGroup& cells);
private:
   virtual void
   buildScalingFactors(
         const CellGroup& cells,
         const MapGroup& map);
   void
   buildLIFT();
   virtual void
   assignMatrices(const CellGroup& cells) = 0;
   void
   allocateFieldsAndRes();
   void
   setResidualsToZero();
};
#endif /* SOLVER_H_ */

