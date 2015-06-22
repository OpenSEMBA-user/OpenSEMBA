/*
 * SolverSIBC.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#include "DGSIBC.h"

DGSIBC::DGSIBC(
      const PMSurfaceSIBC& mat_, const vector<const BoundaryCondition*>& bc,
      const CellGroup& cells, Int*** map_, const Int vmapM_[faces][nfp],
      Real*** ExP_, Real*** EyP_, Real*** EzP_,
      Real*** HxP_, Real*** HyP_, Real*** HzP_) :
      PMSurfaceSIBC(mat_) {
   map = map_;
   ExP = ExP_;
   EyP = EyP_;
   EzP = EzP_;
   HxP = HxP_;
   HyP = HyP_;
   HzP = HzP_;
   for (UInt i = 0; i < faces; i++) {
      for (UInt j = 0; j < nfp; j++) {
         vmapM[i][j] = vmapM_[i][j];
      }
   }
   // Inits sibc positions.
   vector<pair<UInt, UInt> > efList0, efListD;
   efList0.reserve(cells.getLocalSize());
   efListD.reserve(cells.getLocalSize());
   for (UInt i = 0; i < bc.size(); i++) {
      UInt id = bc[i]->getCell()->getId();
      UInt f = bc[i]->getFace();
      if (!bc[i]->isBack()) {
         pair<UInt,UInt> aux0(cells.getRelPosOfId(id), f);
         efList0.push_back(aux0);
      } else {
         pair<UInt,UInt> auxD(cells.getRelPosOfId(id), f);
         efListD.push_back(auxD);
      }
      // TODO Only works with flat faces.
      assert(!bc[i]->getCell()->isCurvedFace(f));
      // TODO Can't be a computational domain boundary.
   }
   nE0 = efList0.size();
   elem0 = new UInt[nE0];
   face0 = new UInt[nE0];
   for (UInt i = 0; i < nE0; i++) {
      elem0[i] = efList0[i].first;
      face0[i] = efList0[i].second;
   }
   nED = efListD.size();
   elemD = new UInt[nED];
   faceD = new UInt[nED];
   for (UInt i = 0; i < nED; i++) {
      elemD[i] = efListD[i].first;
      faceD[i] = efListD[i].second;
   }
   // Normal vectors.
   n0 = new CVecR3[nE0];
   for (UInt k = 0; k < nE0; k++) {
      n0[k] = bc[k]->getCell()->sideNormal(face0[k]);
   }
   nD = new CVecR3[nED];
   for (UInt k = 0; k < nED; k++) {
      nD[k] = bc[k]->getCell()->sideNormal(faceD[k]);
   }
   // Allocates magnetic currents.
   nP = getNumberOfPoles();
   Q0 = new CVecR3*[nP];
   rhsQ0 = new CVecR3*[nP];
   resQ0 = new CVecR3*[nP];
   QD = new CVecR3*[nP];
   rhsQD = new CVecR3*[nP];
   resQD = new CVecR3*[nP];
   for (UInt p = 0; p < nP; p++) {
      Q0[p] = new CVecR3[nE0*nfp];
      rhsQ0[p] = new CVecR3[nE0*nfp];
      resQ0[p] = new CVecR3[nE0*nfp];
   }
   for (UInt p = 0; p < nP; p++) {
      QD[p] = new CVecR3[nED*nfp];
      rhsQD[p] = new CVecR3[nED*nfp];
      resQD[p] = new CVecR3[nED*nfp];
   }
   // Polarization fields.
   E0 = new CVecR3[nE0*nfp];
   ED = new CVecR3[nED*nfp];
}


DGSIBC::~DGSIBC() {

}

void
DGSIBC::computeRHSMagneticPolarizationCurrents(
      const Field<Real,3>& H,
      const UInt e1, const UInt e2) {
   UInt e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(e)
#endif
   for (e = 0; e < nE0; e++) {
      UInt e0 = elem0[e]; // Element-face pair.
      UInt f0 = face0[e];
      UInt eD = elemD[e];
      UInt fD = faceD[e];
      if (e1 <= e0 && e0 < e2) {
         for (UInt p = 0; p < nP; p++) {
            UInt i = e * nfp;
            for (UInt j = 0; j < nfp; j++) {
               Int vM = e0 * np + vmapM[f0][j];
               const CVecR3 H0(H(x)[vM],H(y)[vM],H(z)[vM]);
               UInt vP = map[e0][f0][j]; // Field coeff pos.
               const CVecR3 HD(HxP[eD][fD][vP], HyP[eD][fD][vP], HzP[eD][fD][vP]);
               rhsQ0[p][i] =  (Q0[p][i] * pole_[p])
                           + (n0[e] ^ H0) * Z_[p](0,0) + (n0[e] ^ HD) * Z_[p](0,1);
               i++;
            }
         }
      }
   }
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(e)
#endif
   for (e = 0; e < nED; e++) {
      UInt e0 = elem0[e]; // Element-face pair.
      UInt f0 = face0[e];
      UInt eD = elemD[e];
      UInt fD = faceD[e];
      if (e1 <= eD && eD < e2) {
         for (UInt p = 0; p < nP; p++) {
            UInt i = e * nfp; // Node number.
            for (UInt j = 0; j < nfp; j++) {
               UInt vM = eD * np + vmapM[fD][j]; // Field coeff pos.
               const CVecR3
               HD(H(x)[vM],H(y)[vM],H(z)[vM]);
               UInt vP = map[eD][fD][j]; // Field coeff pos.
               const CVecR3
               H0(HxP[e0][f0][vP], HyP[e0][f0][vP], HzP[e0][f0][vP]);
               rhsQD[p][i] = QD[p][i] * pole_[p]
                - (nD[e] ^ H0) * Z_[p](1,0) - (nD[e] ^ HD) * Z_[p](1,1);
               i++;
            }
         }
      }
   }
}

void
DGSIBC::computeRHSMagnetic(
      Field<Real,3>& rhsH,
      const Field<Real,3>& H,
      const UInt e1, const UInt e2) const {

}

void
DGSIBC::computePolarizationFields(
      const Real* Hx, const Real* Hy,	const Real* Hz,
      const UInt e1, const UInt e2) {
   for (UInt e = 0; e < nE0; e++) {
      UInt e0 = elem0[e];
      UInt f0 = face0[e];
      UInt eD = elemD[e];
      UInt fD = faceD[e];
      if (e1 <= e0 && e0 < e2) {
         for (UInt j = 0; j < nfp; j++) {
            Int vM = e0 * np + vmapM[f0][j];
            CVecR3 H0(Hx[vM],Hy[vM],Hz[vM]);
            UInt vP = map[eD][fD][j]; // Field coeff pos.
            CVecR3
            HD(HxP[eD][fD][vP], HyP[eD][fD][vP], HzP[eD][fD][vP]);
            UInt i = e * nfp;
#				warning "Ignoring Zinfinite"
            //				E0[i] = (n0[e] ^ H0) * mat.Zinfinite[0]
            //				 + (n0[e] ^ HD) * mat.Zinfinite[1];
            for (UInt p = 0; p < nP; p++) {
               E0[i] += Q0[p][i];
            }
            i++;
         }
      }
   }
   for (UInt e = 0; e < nED; e++) {
      UInt e0 = elem0[e];
      UInt f0 = face0[e];
      UInt eD = elemD[e];
      UInt fD = faceD[e];
      if (e1 <= e0 && e0 < e2) {
         for (UInt j = 0; j < nfp; j++) {
            Int vM = eD * np + vmapM[fD][j];
            CVecR3 H0(Hx[vM],Hy[vM],Hz[vM]);
            UInt vP = map[e0][f0][j]; // Field coeff pos.
            CVecR3
            HD(HxP[e0][f0][vP], HyP[e0][f0][vP], HzP[e0][f0][vP]);
            UInt i = e * nfp;
#				warning "Ignoring Zinfinite"
            //				ED[i] = - (nD[e] ^ H0) * mat.Zinfinite[2]
            //				 - (nD[e] ^ HD) * mat.Zinfinite[3];
            for (UInt p = 0; p < nP; p++) {
               ED[i] += QD[p][i];
            }
            i++;
         }
      }
   }
}

void
DGSIBC::addJumps(
      Field<Real,3>& dE, Field<Real,3>& dH,
      Field<Real,3>& E, Field<Real,3>& H,
      const UInt e1, const UInt e2) {
   // Updates E0 and ED.
   computePolarizationFields(H(x),H(y),H(z),e1,e2);
   // Updates jumps with polarized fields.
   UInt i, f, e, j, n;
   for (UInt lE = 0; lE < nE0; lE++) {
      f = face0[lE];
      e = elem0[lE];
      if (e >= e1 && e < e2) {
         n = (e * faces + f) * nfp;
         i = lE * nfp;
         for (j = 0; j < nfp; j++) {
            Int vM = e * np + vmapM[f][j];
            dE.set(x)[n] = 2.0 * (E(x)[vM] - E0[i](0));
            dE.set(y)[n] = 2.0 * (E(y)[vM] - E0[i](1));
            dE.set(z)[n] = 2.0 * (E(z)[vM] - E0[i](2));
            dH.set(x)[n] = 0.0;
            dH.set(y)[n] = 0.0;
            dH.set(z)[n] = 0.0;
            i++;
            n++;
         }
      }
   }
   for (UInt lE = 0; lE < nED; lE++) {
      f = faceD[lE];
      e = elemD[lE];
      if (e >= e1 && e < e2) {
         n = (e * faces + f) * nfp;
         i = nfp * lE;
         for (j = 0; j < nfp; j++) {
            Int vM = e * np + vmapM[f][j];
            dE.set(x)[n] = 2.0 * (E(x)[vM] - ED[i](0));
            dE.set(y)[n] = 2.0 * (E(y)[vM] - ED[i](1));
            dE.set(z)[n] = 2.0 * (E(z)[vM] - ED[i](2));
            dH.set(x)[n] = 0.0;
            dH.set(y)[n] = 0.0;
            dH.set(z)[n] = 0.0;
            i++;
            n++;
         }
      }
   }
}

void
DGSIBC::addRHSToRes(
      const UInt e1, const UInt e2,
      const Real rka,
      const Real dt) {
   for (UInt e = 0; e < nE0; e++) {
      if (e >= e1 && e < e2) {
         for (UInt p = 0; p < nP; p++) {
            UInt i = e*nfp;
            for (UInt j = 0; j < nfp; j++) {
               resQ0[p][i] = resQ0[p][i] * rka + rhsQ0[p][i] * dt;
               i++;
            }
         }
      }
   }
   for (UInt e = 0; e < nED; e++) {
      if (e >= e1 && e < e2) {
         for (UInt p = 0; p < nP; p++) {
            UInt i = e*nfp;
            for (UInt j = 0; j < nfp; j++) {
               resQD[p][i] = resQD[p][i] * rka + rhsQD[p][i] * dt;
               i++;
            }
         }
      }
   }
}

void
DGSIBC::updateWithRes(
      const UInt e1, const UInt e2, const Real rkb) {
   for (UInt e = 0; e < nE0; e++) {
      if (e >= e1 && e < e2) {
         for (UInt p = 0; p < nP; p++) {
            UInt i = e*nfp;
            for (UInt j = 0; j < nfp; j++) {
               Q0[p][i] += resQ0[p][i] * rkb;
            }
         }
      }
   }
   for (UInt e = 0; e < nED; e++) {
      if (e >= e1 && e < e2) {
         for (UInt p = 0; p < nP; p++) {
            UInt i = e*nfp;
            for (UInt j = 0; j < nfp; j++) {
               QD[p][i] += resQD[p][i] * rkb;
            }
         }
      }
   }
}

void
DGSIBC::computeRHSElectric(
      Field<Real, 3>& rhsE,
      const Field<Real, 3>& E,
      const UInt e1, const UInt e2) const {
}

void
DGSIBC::computeRHSElectricPolarizationCurrents(
      const Field<Real, 3>& E,
      const UInt e1, const UInt e2) {
}
