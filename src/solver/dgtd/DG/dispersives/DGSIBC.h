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
         int ***map_,
         const int vmapM[faces][nfp],
         double ***ExP_,
         double ***EyP_,
         double ***EzP_,
         double ***HxP_,
         double ***HyP_,
         double ***HzP_);
   virtual ~DGSIBC();
   void
   computeRHSElectricPolarizationCurrents(
         const Field<double,3>& E,
         const uint e1,
         const uint e2);
   void
   computeRHSMagneticPolarizationCurrents(
         const Field<double,3>& H,
         const uint e1,
         const uint e2);
   void
   computeRHSElectric(
         Field<double,3>& rhsE,
         const Field<double,3>& E,
         const uint e1, const uint e2) const;
   void
   computeRHSMagnetic(
         Field<double,3>& rhsH,
         const Field<double,3>& H,
         const uint e1, const uint e2) const;
   void
   addJumps(
         Field<double,3>& dE, Field<double,3>& dH,
         Field<double,3>& E, Field<double,3>& H,
         const uint e1, const uint e2);
   void
   addRHSToRes(
         const uint e1,
         const uint e2,
         const double rka,
         const double dt);
   void
   updateWithRes(
         const uint e1,
         const uint e2,
         const double rkb);
private:
   int ***map;
   double ***ExP, ***EyP, ***EzP, ***HxP, ***HyP, ***HzP;
   int vmapM[faces][nfp];
   uint nP;
   uint nE0, nED;
   uint *elem0, *elemD;
   uint *face0, *faceD;
   CVecR3 *n0, *nD;
   CVecR3 **Q0, **rhsQ0, **resQ0;
   CVecR3 **QD, **rhsQD, **resQD;
   CVecR3 *E0, *ED;
   void
   computePolarizationFields(
         const double *Hx, const double *Hy, const double *Hz,
         const uint e1, const uint e2);

};

#endif /* SOLVERSIBC_H_ */
