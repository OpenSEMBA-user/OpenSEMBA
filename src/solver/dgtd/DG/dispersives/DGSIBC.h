/*
 * SolverSIBC.h
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#ifndef DGSIBC_H_
#define DGSIBC_H_

#include "DGDispersive.h"
#include "../../core/BoundaryCondition.h"

#define SOLVER_USE_STATIC_LIMIT_FOR_SIBC

class DGSIBC : public DGDispersive, public PMSurfaceSIBC {
   friend class PMSurface;
public:
   DGSIBC(
         const PMSurfaceSIBC& mat_,
         const CellGroup& cells,
         Int ***map_,
         const Int vmapM[faces][nfp],
         Real ***ExP_,
         Real ***EyP_,
         Real ***EzP_,
         Real ***HxP_,
         Real ***HyP_,
         Real ***HzP_);
   virtual ~DGSIBC();
   void
   computeRHSElectricPolarizationCurrents(
         const Field<Real,3>& E,
         const UInt e1,
         const UInt e2);
   void
   computeRHSMagneticPolarizationCurrents(
         const Field<Real,3>& H,
         const UInt e1,
         const UInt e2);
   void
   computeRHSElectric(
         Field<Real,3>& rhsE,
         const Field<Real,3>& E,
         const UInt e1, const UInt e2) const;
   void
   computeRHSMagnetic(
         Field<Real,3>& rhsH,
         const Field<Real,3>& H,
         const UInt e1, const UInt e2) const;
   void
   addJumps(
         Field<Real,3>& dE, Field<Real,3>& dH,
         Field<Real,3>& E, Field<Real,3>& H,
         const UInt e1, const UInt e2);
   void
   addRHSToRes(
         const UInt e1,
         const UInt e2,
         const Real rka,
         const Real dt);
   void
   updateWithRes(
         const UInt e1,
         const UInt e2,
         const Real rkb);
private:
   Int ***map;
   Real ***ExP, ***EyP, ***EzP, ***HxP, ***HyP, ***HzP;
   Int vmapM[faces][nfp];
   UInt nP;
   UInt nE0, nED;
   UInt *elem0, *elemD;
   UInt *face0, *faceD;
   CVecR3 *n0, *nD;
   CVecR3 **Q0, **rhsQ0, **resQ0;
   CVecR3 **QD, **rhsQD, **resQD;
   CVecR3 *E0, *ED;
   void
   computePolarizationFields(
         const Real *Hx, const Real *Hy, const Real *Hz,
         const UInt e1, const UInt e2);

};

#endif /* SOLVERSIBC_H_ */
