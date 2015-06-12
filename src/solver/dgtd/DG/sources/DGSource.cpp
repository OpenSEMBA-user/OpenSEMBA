/*
 * DGSource.cpp
 *
 *  Created on: Sep 2, 2013
 *      Author: luis
 */

#include "DGSource.h"

DGSource::DGSource() {
   ExTInc = NULL;
   EyTInc = NULL;
   EzTInc = NULL;
   HxTInc = NULL;
   HyTInc = NULL;
   HzTInc = NULL;
   ExSInc = NULL;
   EySInc = NULL;
   EzSInc = NULL;
   HxSInc = NULL;
   HySInc = NULL;
   HzSInc = NULL;
   ExIncNB = NULL;
   EyIncNB = NULL;
   EzIncNB = NULL;
   HxIncNB = NULL;
   HyIncNB = NULL;
   HzIncNB = NULL;
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

void
DGSource::initSource(
      const vector<const BoundaryCondition*>& bc,
      const MapGroup& map,
      const CellGroup& cells,
      FieldR3& dE, FieldR3& dH,
      const int vmapM[faces][nfp]) {
   vector<pair<uint, uint> > total, scatt, totalNotBacked;
   total = getElemFaces(bc, map, cells, totalField);
   scatt = getElemFaces(bc, map, cells, scatteredField);
   totalNotBacked = getElemFaces(bc, map, cells, totalFieldNotBacked);
   nETF = total.size();
   ExTInc  = new double[nETF*nfp];
   EyTInc  = new double[nETF*nfp];
   EzTInc  = new double[nETF*nfp];
   HxTInc  = new double[nETF*nfp];
   HyTInc  = new double[nETF*nfp];
   HzTInc  = new double[nETF*nfp];
   for (uint i = 0; i < nETF*nfp; i++) {
      ExTInc[i] = 0.0;
      EyTInc[i] = 0.0;
      EzTInc[i] = 0.0;
      HxTInc[i] = 0.0;
      HyTInc[i] = 0.0;
      HzTInc[i] = 0.0;
   }
   nESF = scatt.size();
   ExSInc  = new double[nESF*nfp];
   EySInc  = new double[nESF*nfp];
   EzSInc  = new double[nESF*nfp];
   HxSInc  = new double[nESF*nfp];
   HySInc  = new double[nESF*nfp];
   HzSInc  = new double[nESF*nfp];
   for (uint i = 0; i < nESF*nfp; i++) {
      ExSInc[i] = 0.0;
      EySInc[i] = 0.0;
      EzSInc[i] = 0.0;
      HxSInc[i] = 0.0;
      HySInc[i] = 0.0;
      HzSInc[i] = 0.0;
   }
   nETFNB = totalNotBacked.size();
   ExIncNB = new double[nETFNB*nfp];
   EyIncNB = new double[nETFNB*nfp];
   EzIncNB = new double[nETFNB*nfp];
   HxIncNB = new double[nETFNB*nfp];
   HyIncNB = new double[nETFNB*nfp];
   HzIncNB = new double[nETFNB*nfp];
   for (uint i = 0; i < nETFNB*nfp; i++) {
      ExIncNB[i] = 0.0;
      EyIncNB[i] = 0.0;
      EzIncNB[i] = 0.0;
      HxIncNB[i] = 0.0;
      HyIncNB[i] = 0.0;
      HzIncNB[i] = 0.0;
   }
   // Allocates and sets jumps pointers.
   // The pointers point to the beginning of the face that they have to
   // update on each iteration.
   dExT = new double*[nETF];
   dEyT = new double*[nETF];
   dEzT = new double*[nETF];
   dHxT = new double*[nETF];
   dHyT = new double*[nETF];
   dHzT = new double*[nETF];
   for (uint j = 0; j < nETF; j++) {
      uint e = total[j].first;
      uint f = total[j].second;
      uint pos = e * nfp * faces + f * nfp;
      dExT[j] = &dE.set(x)[pos];
      dEyT[j] = &dE.set(y)[pos];
      dEzT[j] = &dE.set(z)[pos];
      dHxT[j] = &dH.set(x)[pos];
      dHyT[j] = &dH.set(y)[pos];
      dHzT[j] = &dH.set(z)[pos];
   }
   dExS = new double*[nESF];
   dEyS = new double*[nESF];
   dEzS = new double*[nESF];
   dHxS = new double*[nESF];
   dHyS = new double*[nESF];
   dHzS = new double*[nESF];
   for (uint j = 0; j < nESF; j++) {
      uint e = scatt[j].first;
      uint f = scatt[j].second;
      uint pos = e * nfp * faces + f * nfp;
      dExS[j] = &dE.set(x)[pos];
      dEyS[j] = &dE.set(y)[pos];
      dEzS[j] = &dE.set(z)[pos];
      dHxS[j] = &dH.set(x)[pos];
      dHyS[j] = &dH.set(y)[pos];
      dHzS[j] = &dH.set(z)[pos];
   }
   dExTNB = new double*[nETFNB];
   dEyTNB = new double*[nETFNB];
   dEzTNB = new double*[nETFNB];
   dHxTNB = new double*[nETFNB];
   dHyTNB = new double*[nETFNB];
   dHzTNB = new double*[nETFNB];
   for (uint j = 0; j < nETFNB; j++) {
      uint e = totalNotBacked[j].first;
      uint f = totalNotBacked[j].second;
      uint pos = e * (nfp*faces) + f * nfp;
      dExTNB[j] = &dE.set(x)[pos];
      dEyTNB[j] = &dE.set(y)[pos];
      dEzTNB[j] = &dE.set(z)[pos];
      dHxTNB[j] = &dH.set(x)[pos];
      dHyTNB[j] = &dH.set(y)[pos];
      dHzTNB[j] = &dH.set(z)[pos];
   }
   // List of elements.
   ETFe = new uint[nETF];
   for (uint i = 0; i < nETF; i++) {
      ETFe[i] = total[i].first;
   }
   ESFe = new uint[nESF];
   for (uint i = 0; i < nESF; i++) {
      ESFe[i] = scatt[i].first;
   }
   ETFNBe = new uint[nETFNB];
   for (uint i = 0; i < nETFNB; i++) {
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

vector<pair<uint, uint> >
DGSource::getElemFaces(
      const vector<const BoundaryCondition*>& bc,
      const MapGroup& map,
      const CellGroup& cells,
      const BackingType type) const {
   vector<pair<uint, uint> > res;
   for (uint i = 0; i < bc.size(); i++) {
      uint id1 = bc[i]->getCell()->getId();
      uint f1 = bc[i]->getFace();
      uint id2 = map.getNeighbour(id1,f1)->getId();
      uint f2 = map.getVolToF(id1, f1);
      switch (type) {
      case totalField:
         if (!map.isDomainBoundary(id1,f1) && cells.isLocalId(id1)) {
            uint e1 = cells.getRelPosOfId(id1);
            res.push_back(pair<uint,uint>(e1, f1));
         }
         break;
      case scatteredField:
         if (!map.isDomainBoundary(id1,f1) && cells.isLocalId(id2)) {
            uint e2 = cells.getRelPosOfId(id2);
            res.push_back(pair<uint,uint>(e2, f2));
         }
         break;
      case totalFieldNotBacked:
         if (map.isDomainBoundary(id1,f1) && cells.isLocalId(id1)) {
            uint e1 = cells.getRelPosOfId(id1);
            res.push_back(pair<uint,uint>(e1, f1));
         }
         break;
      }
   }
   return res;
}


void
DGSource::addJumps(
      const uint e1,
      const uint e2) {
   uint j, k, pos;
   // Total field jumps.
   for (j = 0; j < nETF; j++) {
      if (e1 <= ETFe[j] && ETFe[j] < e2) {
         for (k = 0; k < nfp; k++) {
            pos = j * nfp + k;
            dExT[j][k] -= ExTInc[pos];
            dEyT[j][k] -= EyTInc[pos];
            dEzT[j][k] -= EzTInc[pos];
            dHxT[j][k] -= HxTInc[pos];
            dHyT[j][k] -= HyTInc[pos];
            dHzT[j][k] -= HzTInc[pos];
         }
      }
   }
   // Scatt field jumps.
   for (j = 0; j < nESF; j++) {
      if (e1 <= ESFe[j] && ESFe[j] < e2) {
         for (k = 0; k < nfp; k++) {
            pos = j * nfp + k;
            dExS[j][k] += ExSInc[pos];
            dEyS[j][k] += EySInc[pos];
            dEzS[j][k] += EzSInc[pos];
            dHxS[j][k] += HxSInc[pos];
            dHyS[j][k] += HySInc[pos];
            dHzS[j][k] += HzSInc[pos];
         }
      }
   }
   // Computes TFNB excitation jumps.
   for (j = 0; j < nETFNB; j++) {
      if (e1 <= ETFNBe[j] && ETFNBe[j] < e2) {
         for (k = 0; k < nfp; k++) {
            pos = j * nfp + k;
            // Total field jumps of not backed elements.
            // The inc. field is substracted to computed by the SMA
            dExTNB[j][k] -= ExIncNB[pos];
            dEyTNB[j][k] -= EyIncNB[pos];
            dEzTNB[j][k] -= EzIncNB[pos];
            dHxTNB[j][k] -= HxIncNB[pos];
            dHyTNB[j][k] -= HyIncNB[pos];
            dHzTNB[j][k] -= HzIncNB[pos];
         }
      }
   }
}

CVecR3*
DGSource::initPositions(
      const vector<pair<uint, uint> >& elemFace,
      const CellGroup& cells) const {
   const uint nE = elemFace.size();
   CVecR3 *pos;
   pos = new CartesianVector<double, 3> [nE * nfp];
   for (uint i = 0; i < nE; i++) {
      uint id = cells.getIdOfRelPos(elemFace[i].first);
      uint f = elemFace[i].second;
      for (uint j = 0; j < nfp; j++) {
         pos[i*nfp+j] =
               cells.getPtrToCellWithId(id)->getSideNodePos(f,j);
      }
   }
   return pos;
}

bool
DGSource::isContainedInPlane(
      const CartesianPlane plane,
      const vector<const BoundaryCondition*>& bc) const {
   for (uint i = 0; i < bc.size(); i++) {
      uint face = bc[i]->getFace();
      if (!bc[i]->getCell()->isFaceContainedInPlane(face, plane)) {
         return false;
      }
   }
   return true;
}
