/*
 * DGSource.cpp
 *
 *  Created on: Sep 2, 2013
 *      Author: luis
 */

#include "DGSource.h"

DGSource::DGSource() {
   nETF = 0;
   ETFe = NULL;
   dExT = NULL;
   dEyT = NULL;
   dEzT = NULL;
   dHxT = NULL;
   dHyT = NULL;
   dHzT = NULL;
   nESF = 0;
   ESFe = NULL;
   dExS = NULL;
   dEyS = NULL;
   dEzS = NULL;
   dHxS = NULL;
   dHyS = NULL;
   dHzS = NULL;
   nETFNB = 0;
   ETFNBe = NULL;
   dExTNB = NULL;
   dEyTNB = NULL;
   dEzTNB = NULL;
   dHxTNB = NULL;
   dHyTNB = NULL;
   dHzTNB = NULL;
}

DGSource::~DGSource() {
   // TODO Auto-generated destructor stub
}

void DGSource::initSource(
      FieldR3& dE, FieldR3& dH,
      const Int vmapM[faces][nfp]) {
   vector<pair<UInt, UInt> > total, scatt, totalNotBacked;
//   total = getElemFaces(bc, map, cells, totalField);
//   scatt = getElemFaces(bc, map, cells, scatteredField);
//   totalNotBacked = getElemFaces(bc, map, cells, totalFieldNotBacked);
   nETF = total.size();
   ETInc.set(nETF*nfp, 0.0);
   HTInc.set(nETF*nfp, 0.0);
   nESF = scatt.size();
   ESInc.set(nESF*nfp, 0.0);
   HSInc.set(nESF*nfp, 0.0);
   nETFNB = totalNotBacked.size();
   EIncNB.set(nETFNB*nfp, 0.0);
   HIncNB.set(nETFNB*nfp, 0.0);
   // Allocates and sets jumps pointers.
   // The pointers point to the beginning of the face that they have to
   // update on each iteration.
   dExT = new Real*[nETF];
   dEyT = new Real*[nETF];
   dEzT = new Real*[nETF];
   dHxT = new Real*[nETF];
   dHyT = new Real*[nETF];
   dHzT = new Real*[nETF];
   for (UInt j = 0; j < nETF; j++) {
      UInt e = total[j].first;
      UInt f = total[j].second;
      UInt pos = e * nfp * faces + f * nfp;
      dExT[j] = &dE.set(x)[pos];
      dEyT[j] = &dE.set(y)[pos];
      dEzT[j] = &dE.set(z)[pos];
      dHxT[j] = &dH.set(x)[pos];
      dHyT[j] = &dH.set(y)[pos];
      dHzT[j] = &dH.set(z)[pos];
   }
   dExS = new Real*[nESF];
   dEyS = new Real*[nESF];
   dEzS = new Real*[nESF];
   dHxS = new Real*[nESF];
   dHyS = new Real*[nESF];
   dHzS = new Real*[nESF];
   for (UInt j = 0; j < nESF; j++) {
      UInt e = scatt[j].first;
      UInt f = scatt[j].second;
      UInt pos = e * nfp * faces + f * nfp;
      dExS[j] = &dE.set(x)[pos];
      dEyS[j] = &dE.set(y)[pos];
      dEzS[j] = &dE.set(z)[pos];
      dHxS[j] = &dH.set(x)[pos];
      dHyS[j] = &dH.set(y)[pos];
      dHzS[j] = &dH.set(z)[pos];
   }
   dExTNB = new Real*[nETFNB];
   dEyTNB = new Real*[nETFNB];
   dEzTNB = new Real*[nETFNB];
   dHxTNB = new Real*[nETFNB];
   dHyTNB = new Real*[nETFNB];
   dHzTNB = new Real*[nETFNB];
   for (UInt j = 0; j < nETFNB; j++) {
      UInt e = totalNotBacked[j].first;
      UInt f = totalNotBacked[j].second;
      UInt pos = e * (nfp*faces) + f * nfp;
      dExTNB[j] = &dE.set(x)[pos];
      dEyTNB[j] = &dE.set(y)[pos];
      dEzTNB[j] = &dE.set(z)[pos];
      dHxTNB[j] = &dH.set(x)[pos];
      dHyTNB[j] = &dH.set(y)[pos];
      dHzTNB[j] = &dH.set(z)[pos];
   }
   // List of elements.
   ETFe = new UInt[nETF];
   for (UInt i = 0; i < nETF; i++) {
      ETFe[i] = total[i].first;
   }
   ESFe = new UInt[nESF];
   for (UInt i = 0; i < nESF; i++) {
      ESFe[i] = scatt[i].first;
   }
   ETFNBe = new UInt[nETFNB];
   for (UInt i = 0; i < nETFNB; i++) {
      ETFNBe[i] = totalNotBacked[i].first;
   }
   //
   if (nETF == 0 && nESF == 0 && nETFNB == 0) {
      cerr << endl << "ERROR @ InitSources" << endl;
      cerr << endl << "In source:" << endl;
      printInfo();
      cerr << endl << "No elements were found for excitation." << endl;
   }
}

void
DGSource::addJumps(
      const UInt e1,
      const UInt e2) {
//   UInt j, k, pos;
//   // Total field jumps.
//   for (j = 0; j < nETF; j++) {
//      if (e1 <= ETFe[j] && ETFe[j] < e2) {
//         for (k = 0; k < nfp; k++) {
//            pos = j * nfp + k;
//            dExT[j][k] -= ExTInc[pos];
//            dEyT[j][k] -= EyTInc[pos];
//            dEzT[j][k] -= EzTInc[pos];
//            dHxT[j][k] -= HxTInc[pos];
//            dHyT[j][k] -= HyTInc[pos];
//            dHzT[j][k] -= HzTInc[pos];
//         }
//      }
//   }
//   // Scatt field jumps.
//   for (j = 0; j < nESF; j++) {
//      if (e1 <= ESFe[j] && ESFe[j] < e2) {
//         for (k = 0; k < nfp; k++) {
//            pos = j * nfp + k;
//            dExS[j][k] += ExSInc[pos];
//            dEyS[j][k] += EySInc[pos];
//            dEzS[j][k] += EzSInc[pos];
//            dHxS[j][k] += HxSInc[pos];
//            dHyS[j][k] += HySInc[pos];
//            dHzS[j][k] += HzSInc[pos];
//         }
//      }
//   }
//   // Computes TFNB excitation jumps.
//   for (j = 0; j < nETFNB; j++) {
//      if (e1 <= ETFNBe[j] && ETFNBe[j] < e2) {
//         for (k = 0; k < nfp; k++) {
//            pos = j * nfp + k;
//            // Total field jumps of not backed elements.
//            // The inc. field is substracted to computed by the SMA
//            dExTNB[j][k] -= ExIncNB[pos];
//            dEyTNB[j][k] -= EyIncNB[pos];
//            dEzTNB[j][k] -= EzIncNB[pos];
//            dHxTNB[j][k] -= HxIncNB[pos];
//            dHyTNB[j][k] -= HyIncNB[pos];
//            dHzTNB[j][k] -= HzIncNB[pos];
//         }
//      }
//   }
}

//CVecR3*
//DGSource::initPositions(
//      const vector<pair<UInt, UInt> >& elemFace,
//      const CellGroup& cells) const {
//   const UInt nE = elemFace.size();
//   CVecR3 *pos;
//   pos = new CVecR3 [nE * nfp];
//   for (UInt i = 0; i < nE; i++) {
//      UInt id = cells.getIdOfRelPos(elemFace[i].first);
//      UInt f = elemFace[i].second;
//      for (UInt j = 0; j < nfp; j++) {
//         pos[i*nfp+j] =
//               cells.getPtrToCellWithId(id)->getSideNodePos(f,j);
//      }
//   }
//   return pos;
//}
