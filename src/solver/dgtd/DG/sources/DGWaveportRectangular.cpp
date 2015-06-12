/*
 * DGWaveportRectangular.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: luis
 */

#include "DGWaveportRectangular.h"

DGWaveportRectangular::DGWaveportRectangular(
      const Waveport& wp,
      const vector<const BoundaryCondition*>& bc,
      const MapGroup& map,
      const CellGroup& cells,
      FieldR3& dE, FieldR3& dH,
      const int vmapM[faces][nfp]) :
      Waveport(wp) {
   initSource(bc, map, cells, dE, dH, vmapM);
   // Computes positions.
   vector<pair<uint, uint> > total;
   total = getElemFaces(bc, map, cells, totalField);
   posTF = initPositions(total, cells);
   if (!checkNormalsAreEqual(total, cells)) {
      cerr << endl << "Total Normals are different" << endl;
   }
   vector<pair<uint,uint> > scatt;
   scatt = getElemFaces(bc, map, cells, scatteredField);
   posSF = initPositions(scatt, cells);
   if (!checkNormalsAreEqual(scatt, cells)) {
      cerr << endl << "Scatt Normals are different" << endl;
   }
   vector<pair<uint, uint> > totalNB;
   totalNB = getElemFaces(bc, map, cells, totalFieldNotBacked);
   posTFNB = initPositions(totalNB, cells);
   if (!checkNormalsAreEqual(totalNB, cells)) {
      cerr << endl << "Total Not Backed Normals are different" << endl;
   }
   // Compute waveport size.
   double zMax, zMin, yMax, yMin;
   if (nETF != 0) {
      zMax = posTF[0](2);
      zMin = posTF[0](2);
      yMax = posTF[0](1);
      yMin = posTF[0](1);
   } else {
      zMax = posTFNB[0](2);
      zMin = posTFNB[0](2);
      yMax = posTFNB[0](1);
      yMin = posTFNB[0](1);
   }
   for (uint i = 0; i < (nETF*nfp); i++) {
      if (posTF[i](2) > zMax) {
         zMax = posTF[i](2);
      }
      if (posTF[i](2) < zMin) {
         zMin = posTF[i](2);
      }
      if (posTF[i](1) > yMax) {
         yMax = posTF[i](1);
      }
      if (posTF[i](1) < yMin) {
         yMin = posTF[i](1);
      }
   }
   for (uint i = 0; i < (nETFNB*nfp); i++) {
      if (posTFNB[i](2) > zMax) {
         zMax = posTFNB[i](2);
      }
      if (posTFNB[i](2) < zMin) {
         zMin = posTFNB[i](2);
      }
      if (posTFNB[i](1) > yMax) {
         yMax = posTFNB[i](1);
      }
      if (posTFNB[i](1) < yMin) {
         yMin = posTFNB[i](1);
      }
   }
//   if (getSymXY() == Waveport::none) {
//      width = zMax - zMin;
//   } else {
//      width = (zMax - zMin) * 2.0;
//   }
//   if (getSymZX() == Waveport::none) {
//      height = yMax - yMin;
//   } else {
//      height = (yMax - yMin) * 2.0;
//   }
//   // Displaces origin to center of waveguide.
//   if (getSymXY() != Waveport::none) {
//      for (uint i = 0; i < nETF*nfp; i++) {
//         posTF[i](2) += width / 2.0;
//      }
//      for (uint i = 0; i < nESF*nfp; i++) {
//         posSF[i](2) += width / 2.0;
//      }
//      for (uint i = 0; i < nETFNB*nfp; i++) {
//         posTFNB[i](2) += width / 2.0;
//      }
//   }
//   if (getSymZX() != Waveport::none) {
//      for (uint i = 0; i < nETF*nfp; i++) {
//         posTF[i](1) += height / 2.0;
//      }
//      for (uint i = 0; i < nESF*nfp; i++) {
//         posSF[i](1) += height / 2.0;
//      }
//      for (uint i = 0; i < nETFNB*nfp; i++) {
//         posTFNB[i](2) += height / 2.0;
//      }
//   }
   assert(false);
#  warning "Waveports are not being ctrted."
   // Stores modes.
   excitationMode = getExcitationMode();
   if (excitationMode != Waveport::TE) {
      cerr << endl << "ERROR @ DGWaveportRectangular" << endl;
      cerr << endl << "Non TE mode not supported yet." << endl;
      assert(false);
      exit(-1);
   }
   // Computes kcm.
   kcm = sqrt(pow((double) getMode().first * M_PI/width, 2)
         + pow((double) getMode().second * M_PI/height, 2));
   intrinsicImpedance = sqrt(VACUUM_PERMEABILITY / VACUUM_PERMITTIVITY);
   gammaMSum = 0.0;
}

DGWaveportRectangular::~DGWaveportRectangular() {

}

void
DGWaveportRectangular::computeExcitation(
      const double time,
      const double minDT) {
   computeExcitationField(
         ExTInc, EyTInc, EzTInc, HxTInc, HyTInc, HzTInc,
         posTF, nETF, time, minDT);
   computeExcitationField(
         ExSInc, EySInc, EzSInc, HxSInc, HySInc, HzSInc,
         posSF, nESF, time, minDT);
   computeExcitationField(
         ExIncNB, EyIncNB, EzIncNB, HxIncNB, HyIncNB, HzIncNB,
         posTFNB, nETFNB, time, minDT);
}

void
DGWaveportRectangular::printInfo() const {
   cout << "DGWaveportRectangular::printInfo" << endl;
   cout << "TO BE DONE." << endl;
   // TODO DGWaveportRectangular::printInfo stub.
}

void
DGWaveportRectangular::computeExcitationField(
      double* ExInc,
      double* EyInc,
      double* EzInc,
      double* HxInc,
      double* HyInc,
      double* HzInc,
      const CVecD3* pos,
      const uint nE,
      const double time,
      const double minDT) {
   const double kcmSq = kcm * kcm;
   //	double f = getGauss(time, amplitude,delay,spread);
   double gamma0f = getMagnitude()->evaluate(time) / SPEED_OF_LIGHT;
   //	double gammaMf =
   //	 getNumericalGammaMGauss(time,minDT, amplitude,delay,spread, kcm);
   if (excitationMode == Waveport::TE) {
      const uint nFields = nfp * nE;
      const double mConst = M_PI * getMode().first / width;
      const double nConst = M_PI * getMode().second / height;
      for (uint i = 0; i < nFields; i++) {
         const double yD = pos[i](1);
         const double zD = pos[i](2);
         // --- Electric field, TE ---
         //			ExInc[i] = 0.0;
         EyInc[i] = gamma0f * intrinsicImpedance / kcmSq
               * mConst * sin(mConst * zD) * cos(nConst * yD);
         EzInc[i] = gamma0f * intrinsicImpedance / kcmSq
               * nConst * cos(mConst * zD) * sin(nConst * yD);
         // --- Magnetic field, TE ---
         //			HxInc[i] = f * cos(mConst * zD) * cos(nConst * yD);
         //			HyInc[i] = gammaMf / kcmSq
         //			 * nConst * cos(mConst * zD) * sin(nConst * yD);
         //			HzInc[i] = gammaMf / kcmSq
         //			 * mConst * sin(mConst * zD) * cos(nConst * yD);
      }
   } else {
      cerr << endl << "ERROR @ DGWaveportRectangular." << endl;
      cerr << endl << "TE modes only." << endl;
      assert(false);
      exit(-1);
   }
}
