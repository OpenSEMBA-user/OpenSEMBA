/*
 * SolverSIBC.h
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#ifndef DGSIBC_H_
#define DGSIBC_H_

#include "DGDispersive.h"
#include "physicalModel/PMSurfaceSIBC.h"

#define SOLVER_USE_STATIC_LIMIT_FOR_SIBC

class DGSIBC : public DGDispersive, public PMSurfaceSIBC {
   friend class PMSurface;
public:
   DGSIBC(
         const PMSurfaceSIBC& mat_,
         Int ***map_,
         const Int vmapM[faces][nfp],
         Real ***ExP_,
         Real ***EyP_,
         Real ***EzP_,
         Real ***HxP_,
         Real ***HyP_,
         Real ***HzP_);
   virtual ~DGSIBC();
   void computeRHSElectricPolarizationCurrents(
         const FieldR3& E,
         const UInt e1,
         const UInt e2);
   void computeRHSMagneticPolarizationCurrents(
         const FieldR3& H,
         const UInt e1,
         const UInt e2);
   void computeRHSElectric(
         FieldR3& rhsE,
         const FieldR3& E,
         const UInt e1, const UInt e2) const;
   void computeRHSMagnetic(
         FieldR3& rhsH,
         const FieldR3& H,
         const UInt e1, const UInt e2) const;
   void addJumps(
         FieldR3& dE, FieldR3& dH,
         FieldR3& E, FieldR3& H,
         const UInt e1, const UInt e2);
   void addRHSToRes(
         const UInt e1,
         const UInt e2,
         const Real rka,
         const Real dt);
   void updateWithRes(
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
   void computePolarizationFields(
         const Real *Hx, const Real *Hy, const Real *Hz,
         const UInt e1, const UInt e2);
};

#endif /* SOLVERSIBC_H_ */
