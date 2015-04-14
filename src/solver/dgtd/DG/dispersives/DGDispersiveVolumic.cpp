/*
 * SolverDispersive.cpp
 *
 * Created on: Sep 11, 2012
 *   Author: luis
 */

#include "DGDispersiveVolumic.h"

#include "../DG.h"

DGDispersiveVolumic::DGDispersiveVolumic() {
   nElem = 0;
   dof = 0;
   elem = NULL;
   drudeDof = 0;
}


DGDispersiveVolumic::DGDispersiveVolumic(
      const PMVolumeDispersive& mat,
      const CellGroup& cells) :
         PMVolumeDispersive(mat) {
   build(cells);
}

DGDispersiveVolumic::~DGDispersiveVolumic() {

}

void
DGDispersiveVolumic::updateWithRes(
      const uint e1,
      const uint e2,
      const double rkb) {
   uint i, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,e)
#endif
   for (i = 0; i < dof; i++) {
      e = elem[(i / np) % nElem];
      if (e1 <= e && e < e2) {
         P.set(x)[i] += resP(x)[i] * rkb;
         P.set(y)[i] += resP(y)[i] * rkb;
         P.set(z)[i] += resP(z)[i] * rkb;
      }
   }
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,e)
#endif
   for (i=0; i < drudeDof; i++) {
      e = elem[(i / np) % nElem];
      if (e1 <= e && e < e2) {
         J.set(x)[i] += resJ(x)[i] * rkb;
         J.set(y)[i] += resJ(y)[i] * rkb;
         J.set(z)[i] += resJ(z)[i] * rkb;
      }
   }
}

void
DGDispersiveVolumic::addRHSToRes(
      const uint e1,
      const uint e2,
      const double rka,
      const double dt) {
   uint i, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,e)
#endif
   for (i = 0; i < dof; i++) {
      e = elem[(i / np) % nElem];
      if (e1 <= e && e < e2) {
         resP.set(x)[i] *= rka;
         resP.set(y)[i] *= rka;
         resP.set(z)[i] *= rka;
         resP.set(x)[i] += rhsP(x)[i] * dt;
         resP.set(y)[i] += rhsP(y)[i] * dt;
         resP.set(z)[i] += rhsP(z)[i] * dt;
      }
   }
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,e)
#endif
   for (i = 0; i < drudeDof; i++) {
      e = elem[(i / np) % nElem];
      if (e1 <= e && e < e2) {
         resJ.set(x)[i] *= rka;
         resJ.set(y)[i] *= rka;
         resJ.set(z)[i] *= rka;
         resJ.set(x)[i] += rhsJ(x)[i] * dt;
         resJ.set(y)[i] += rhsJ(y)[i] * dt;
         resJ.set(z)[i] += rhsJ(z)[i] * dt;
      }
   }
}

void
DGDispersiveVolumic::computeRHSElectric(
      Field<double,3>& rhsE,
      const Field<double,3>& E,
      const uint e1, const uint e2) const {
   uint i, j, e, n, p;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e,n,p)
#endif
   for (i = 0; i < dof; i++) {
      e = elem[(i / np) % nElem]; // Element number.
      if (e1 <= e && e < e2) {
         p = i / (nElem * np); // Pole number.
         n = i % np; // Node number.
         j = e * np + n; // Field coeff pos.
         rhsE.set(x)[j] -= 2.0 * real(pole[p] * P(x)[i] + residue[p] * E(x)[j]);
         rhsE.set(y)[j] -= 2.0 * real(pole[p] * P(y)[i] + residue[p] * E(y)[j]);
         rhsE.set(z)[j] -= 2.0 * real(pole[p] * P(z)[i] + residue[p] * E(z)[j]);
      }
   }
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e,n,p)
#endif
   for (i = 0; i < drudeDof; i++) {
      // Pole number.
      p = i / (nElem * np);
      // Element number.
      e = elem[(i / np) % nElem];
      if (e1 <= e && e < e2) {
         // Node number.
         n = i % np;
         // Field coefficient position in the general fields vector.
         j = e * np + n;
         // Adds polarization current contribution to the RHS.
         rhsE.set(x)[j] -= 2.0 * real(J(x)[i]);
         rhsE.set(y)[j] -= 2.0 * real(J(y)[i]);
         rhsE.set(z)[j] -= 2.0 * real(J(z)[i]);
      }
   }
}

void DGDispersiveVolumic::computeRHSMagnetic(Field<double, 3>& rhsE,
      const Field<double, 3>& E, const uint e1, const uint e2) const {
}

void
DGDispersiveVolumic::computeRHSElectricPolarizationCurrents(
      const Field<double,3>& E,
      const uint e1, const uint e2) {
   uint i, j, e, n, p;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e,n,p)
#endif
   for (i = 0; i < dof; i++) {
      e = elem[(i / np) % nElem]; // Element number.
      if (e1 <= e && e < e2) {
         p = i / (nElem * np); // Pole number.
         n = i % np; // Node number.
         j = e * np + n; // Field coeff pos.
         rhsP.set(x)[i] = pole[p] * P(x)[i] + residue[p] * E(x)[j];
         rhsP.set(y)[i] = pole[p] * P(y)[i] + residue[p] * E(y)[j];
         rhsP.set(z)[i] = pole[p] * P(z)[i] + residue[p] * E(z)[j];
      }
   }
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e,n,p)
#endif
   for (i = 0; i < drudeDof; i++) {
      // Pole number.
      p = i / (nElem * np);
      // Element number.
      e = elem[(i / np) % nElem];
      if (e1 <= e && e < e2) {
         // Node number.
         n = i % np;
         // Field coefficient position in the general fields vector.
         j = e * np + n;
         // Updates RHS of Polarization currents.
         rhsJ.set(x)[i] = drudePole[p] * J(x)[i] + drudeResidue[p] * E(x)[j];
         rhsJ.set(y)[i] = drudePole[p] * J(y)[i] + drudeResidue[p] * E(y)[j];
         rhsJ.set(z)[i] = drudePole[p] * J(z)[i] + drudeResidue[p] * E(z)[j];
      }
   }
}


void
DGDispersiveVolumic::computeRHSMagneticPolarizationCurrents(
      const Field<double, 3>& E, const uint e1, const uint e2) {
}

void
DGDispersiveVolumic::addJumps(Field<double, 3>& dE,
      Field<double, 3>& dH, Field<double, 3>& E, Field<double, 3>& H,
      const uint e1, const uint e2) {
}

void
DGDispersiveVolumic::build(
      const CellGroup& cells) {
   for (uint p = 0; p < getPoleNumber(); p++) {
      residue[p] *= VACUUM_PERMITTIVITY;
   }
   for (uint p = 0; p < getDrudePoleNumber(); p++) {
      drudeResidue[p] *= VACUUM_PERMITTIVITY;
   }
   // Creates list of elements containing dispersive material.
   vector<uint> rpList;
   rpList.reserve(cells.getLocalSize());
   for (uint e = 0; e < cells.getLocalSize(); e++) {
      uint id = cells.getIdOfRelPos(e);
      const CellTet<ORDER_N>* cell = cells.getPtrToCellWithId(id);
      if (cell->material->getId() == getId()) {
         rpList.push_back(e);
      }
   }
   nElem = rpList.size();
   elem = new uint[nElem];
   for (uint i = 0; i < nElem; i++) {
      elem[i] = rpList[i];
   }
   // Usual poles.
   dof = np * nElem * getPoleNumber();
   P.setSize(dof);
   P.setToZero();
   rhsP.setSize(dof);
   rhsP.setToZero();
   resP.setSize(dof);
   resP.setToZero();
   // Drude poles.
   drudeDof = np * nElem * getDrudePoleNumber();
   J.setSize(drudeDof);
   J.setToZero();
   rhsJ.setSize(drudeDof);
   rhsJ.setToZero();
   resJ.setSize(drudeDof);
   resJ.setToZero();
}

