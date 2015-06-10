/*
 * SolverSIBC.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: luis
 */

#include "DGSIBC.h"

DGSIBC::DGSIBC(
      const PMSurfaceSIBC& mat_, const vector<const BoundaryCondition*>& bc,
      const CellGroup& cells, int*** map_, const int vmapM_[faces][nfp],
      double*** ExP_, double*** EyP_, double*** EzP_,
      double*** HxP_, double*** HyP_, double*** HzP_) :
      PMSurfaceSIBC(mat_) {
   map = map_;
   ExP = ExP_;
   EyP = EyP_;
   EzP = EzP_;
   HxP = HxP_;
   HyP = HyP_;
   HzP = HzP_;
   for (uint i = 0; i < faces; i++) {
      for (uint j = 0; j < nfp; j++) {
         vmapM[i][j] = vmapM_[i][j];
      }
   }
   // Inits sibc positions.
   vector<pair<uint, uint> > efList0, efListD;
   efList0.reserve(cells.getLocalSize());
   efListD.reserve(cells.getLocalSize());
   for (uint i = 0; i < bc.size(); i++) {
      uint id = bc[i]->getCell()->getId();
      uint f = bc[i]->getFace();
      if (!bc[i]->isBack()) {
         pair<uint,uint> aux0(cells.getRelPosOfId(id), f);
         efList0.push_back(aux0);
      } else {
         pair<uint,uint> auxD(cells.getRelPosOfId(id), f);
         efListD.push_back(auxD);
      }
      // TODO Only works with flat faces.
      assert(!bc[i]->getCell()->isCurvedFace(f));
      // TODO Can't be a computational domain boundary.
   }
   nE0 = efList0.size();
   elem0 = new uint[nE0];
   face0 = new uint[nE0];
   for (uint i = 0; i < nE0; i++) {
      elem0[i] = efList0[i].first;
      face0[i] = efList0[i].second;
   }
   nED = efListD.size();
   elemD = new uint[nED];
   faceD = new uint[nED];
   for (uint i = 0; i < nED; i++) {
      elemD[i] = efListD[i].first;
      faceD[i] = efListD[i].second;
   }
   // Normal vectors.
   n0 = new CVecD3[nE0];
   for (uint k = 0; k < nE0; k++) {
      n0[k] = bc[k]->getCell()->sideNormal(face0[k]);
   }
   nD = new CVecD3[nED];
   for (uint k = 0; k < nED; k++) {
      nD[k] = bc[k]->getCell()->sideNormal(faceD[k]);
   }
   // Allocates magnetic currents.
   nP = getNumberOfPoles();
   Q0 = new CVecD3*[nP];
   rhsQ0 = new CVecD3*[nP];
   resQ0 = new CVecD3*[nP];
   QD = new CVecD3*[nP];
   rhsQD = new CVecD3*[nP];
   resQD = new CVecD3*[nP];
   for (uint p = 0; p < nP; p++) {
      Q0[p] = new CVecD3[nE0*nfp];
      rhsQ0[p] = new CVecD3[nE0*nfp];
      resQ0[p] = new CVecD3[nE0*nfp];
   }
   for (uint p = 0; p < nP; p++) {
      QD[p] = new CVecD3[nED*nfp];
      rhsQD[p] = new CVecD3[nED*nfp];
      resQD[p] = new CVecD3[nED*nfp];
   }
   // Polarization fields.
   E0 = new CVecD3[nE0*nfp];
   ED = new CVecD3[nED*nfp];
}


DGSIBC::~DGSIBC() {

}

void
DGSIBC::computeRHSMagneticPolarizationCurrents(
      const Field<double,3>& H,
      const uint e1, const uint e2) {
   uint e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(e)
#endif
   for (e = 0; e < nE0; e++) {
      uint e0 = elem0[e]; // Element-face pair.
      uint f0 = face0[e];
      uint eD = elemD[e];
      uint fD = faceD[e];
      if (e1 <= e0 && e0 < e2) {
         for (uint p = 0; p < nP; p++) {
            uint i = e * nfp;
            for (uint j = 0; j < nfp; j++) {
               int vM = e0 * np + vmapM[f0][j];
               const CVecD3 H0(H(x)[vM],H(y)[vM],H(z)[vM]);
               uint vP = map[e0][f0][j]; // Field coeff pos.
               const CVecD3 HD(HxP[eD][fD][vP], HyP[eD][fD][vP], HzP[eD][fD][vP]);
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
      uint e0 = elem0[e]; // Element-face pair.
      uint f0 = face0[e];
      uint eD = elemD[e];
      uint fD = faceD[e];
      if (e1 <= eD && eD < e2) {
         for (uint p = 0; p < nP; p++) {
            uint i = e * nfp; // Node number.
            for (uint j = 0; j < nfp; j++) {
               uint vM = eD * np + vmapM[fD][j]; // Field coeff pos.
               const CVecD3
               HD(H(x)[vM],H(y)[vM],H(z)[vM]);
               uint vP = map[eD][fD][j]; // Field coeff pos.
               const CVecD3
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
      Field<double,3>& rhsH,
      const Field<double,3>& H,
      const uint e1, const uint e2) const {

}

void
DGSIBC::computePolarizationFields(
      const double* Hx, const double* Hy,	const double* Hz,
      const uint e1, const uint e2) {
   for (uint e = 0; e < nE0; e++) {
      uint e0 = elem0[e];
      uint f0 = face0[e];
      uint eD = elemD[e];
      uint fD = faceD[e];
      if (e1 <= e0 && e0 < e2) {
         for (uint j = 0; j < nfp; j++) {
            int vM = e0 * np + vmapM[f0][j];
            CVecD3 H0(Hx[vM],Hy[vM],Hz[vM]);
            uint vP = map[eD][fD][j]; // Field coeff pos.
            CVecD3
            HD(HxP[eD][fD][vP], HyP[eD][fD][vP], HzP[eD][fD][vP]);
            uint i = e * nfp;
#				warning "Ignoring Zinfinite"
            //				E0[i] = (n0[e] ^ H0) * mat.Zinfinite[0]
            //				 + (n0[e] ^ HD) * mat.Zinfinite[1];
            for (uint p = 0; p < nP; p++) {
               E0[i] += Q0[p][i];
            }
            i++;
         }
      }
   }
   for (uint e = 0; e < nED; e++) {
      uint e0 = elem0[e];
      uint f0 = face0[e];
      uint eD = elemD[e];
      uint fD = faceD[e];
      if (e1 <= e0 && e0 < e2) {
         for (uint j = 0; j < nfp; j++) {
            int vM = eD * np + vmapM[fD][j];
            CVecD3 H0(Hx[vM],Hy[vM],Hz[vM]);
            uint vP = map[e0][f0][j]; // Field coeff pos.
            CVecD3
            HD(HxP[e0][f0][vP], HyP[e0][f0][vP], HzP[e0][f0][vP]);
            uint i = e * nfp;
#				warning "Ignoring Zinfinite"
            //				ED[i] = - (nD[e] ^ H0) * mat.Zinfinite[2]
            //				 - (nD[e] ^ HD) * mat.Zinfinite[3];
            for (uint p = 0; p < nP; p++) {
               ED[i] += QD[p][i];
            }
            i++;
         }
      }
   }
}

void
DGSIBC::addJumps(
      Field<double,3>& dE, Field<double,3>& dH,
      Field<double,3>& E, Field<double,3>& H,
      const uint e1, const uint e2) {
   // Updates E0 and ED.
   computePolarizationFields(H(x),H(y),H(z),e1,e2);
   // Updates jumps with polarized fields.
   uint i, f, e, j, n;
   for (uint lE = 0; lE < nE0; lE++) {
      f = face0[lE];
      e = elem0[lE];
      if (e >= e1 && e < e2) {
         n = (e * faces + f) * nfp;
         i = lE * nfp;
         for (j = 0; j < nfp; j++) {
            int vM = e * np + vmapM[f][j];
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
   for (uint lE = 0; lE < nED; lE++) {
      f = faceD[lE];
      e = elemD[lE];
      if (e >= e1 && e < e2) {
         n = (e * faces + f) * nfp;
         i = nfp * lE;
         for (j = 0; j < nfp; j++) {
            int vM = e * np + vmapM[f][j];
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
      const uint e1, const uint e2,
      const double rka,
      const double dt) {
   for (uint e = 0; e < nE0; e++) {
      if (e >= e1 && e < e2) {
         for (uint p = 0; p < nP; p++) {
            uint i = e*nfp;
            for (uint j = 0; j < nfp; j++) {
               resQ0[p][i] = resQ0[p][i] * rka + rhsQ0[p][i] * dt;
               i++;
            }
         }
      }
   }
   for (uint e = 0; e < nED; e++) {
      if (e >= e1 && e < e2) {
         for (uint p = 0; p < nP; p++) {
            uint i = e*nfp;
            for (uint j = 0; j < nfp; j++) {
               resQD[p][i] = resQD[p][i] * rka + rhsQD[p][i] * dt;
               i++;
            }
         }
      }
   }
}

void
DGSIBC::updateWithRes(
      const uint e1, const uint e2, const double rkb) {
   for (uint e = 0; e < nE0; e++) {
      if (e >= e1 && e < e2) {
         for (uint p = 0; p < nP; p++) {
            uint i = e*nfp;
            for (uint j = 0; j < nfp; j++) {
               Q0[p][i] += resQ0[p][i] * rkb;
            }
         }
      }
   }
   for (uint e = 0; e < nED; e++) {
      if (e >= e1 && e < e2) {
         for (uint p = 0; p < nP; p++) {
            uint i = e*nfp;
            for (uint j = 0; j < nfp; j++) {
               QD[p][i] += resQD[p][i] * rkb;
            }
         }
      }
   }
}

void
DGSIBC::computeRHSElectric(
      Field<double, 3>& rhsE,
      const Field<double, 3>& E,
      const uint e1, const uint e2) const {
}

void
DGSIBC::computeRHSElectricPolarizationCurrents(
      const Field<double, 3>& E,
      const uint e1, const uint e2) {
}
