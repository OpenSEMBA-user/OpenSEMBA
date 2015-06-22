/*
 * DGPlaneWave.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: luis
 */
#include "DGPlaneWave.h"

DGPlaneWave::DGPlaneWave(
 const PlaneWave& pw,
 const MapGroup& map,
 const CellGroup& cells,
 const Comm* comm,
 FieldR3& dE, FieldR3& dH,
 const Int vmapM[faces][nfp]) :
  PlaneWave(pw) {
//	initSource(bc, map, cells, dE, dH, vmapM);
//	initWaveNumberPosition(bc, map, cells, comm, vmapM);
}


DGPlaneWave::~DGPlaneWave() {
	// TODO Auto-generated destructor stub
}

void
DGPlaneWave::printInfo() const {
	cout << " --- DGPlaneWave Info ---" << endl;
	cout << "# ETF: " << nETF << endl;
	cout << "# ESF: " << nESF << endl;
	cout << "# ETFNB: " << nETFNB << endl;
}

void
DGPlaneWave::computeExcitation(
 const Real intTime, const Real minDT) {
	computeExcitationField(
	 ExTInc, EyTInc, EzTInc, HxTInc, HyTInc, HzTInc, kNPosTF, nETF, intTime);
	computeExcitationField(
	 ExSInc, EySInc, EzSInc, HxSInc, HySInc, HzSInc, kNPosSF, nESF, intTime);
	computeExcitationField(
	 ExIncNB, EyIncNB, EzIncNB, HxIncNB, HyIncNB, HzIncNB,
	 kNPosTFNB, nETFNB, intTime);
}

void
DGPlaneWave::computeExcitationField(
 Real* ExInc, Real *EyInc, Real *EzInc,
 Real* HxInc, Real *HyInc, Real *HzInc,
 const Real* vPos,
 const UInt nE,
 const Real intTime) {
	// Computes the plane wve excitation corresponding to the face f.
	// The face contins nfp nodes and it is computed for time intTime.
	const UInt nFields = nfp*nE;
	for (UInt j = 0; j < nFields; j++) {
		Real delayedTime = intTime - vPos[j];
		if (delayedTime >= 0) {
			pair<CVecR3,CVecR3> EHInc = getElectromagneticField(delayedTime);
			ExInc[j] = EHInc.first(0);
			EyInc[j] = EHInc.first(1);
			EzInc[j] = EHInc.first(2);
			HxInc[j] = EHInc.second(0);
			HyInc[j] = EHInc.second(1);
			HzInc[j] = EHInc.second(2);
		} else {
			ExInc[j] = (Real) 0.0;
			EyInc[j] = (Real) 0.0;
			EzInc[j] = (Real) 0.0;
			HxInc[j] = (Real) 0.0;
			HyInc[j] = (Real) 0.0;
			HzInc[j] = (Real) 0.0;
		}
	}
}

void
DGPlaneWave::initWaveNumberPosition(
 const MapGroup& map,
 const CellGroup& cells,
 const Comm* comm,
 const Int vmapM[faces][nfp]) {
	// Generates kNPosTSF for PW excitation.
	// Computes krmin position.
	Real krmin = 0.0;
	bool krminSet = false;
	CVecR3 nodePos;
	// Total field.
	vector<pair<UInt, UInt> > total;
	total = getElemFaces(map, cells, totalField);
	kNPosTF = new Real[nETF * nfp];
	for (UInt j = 0; j < nETF; j++) {
		UInt id = cells.getIdOfRelPos(total[j].first);
		UInt f = total[j].second;
		UInt pos = j * nfp;
		for (UInt k = 0; k < nfp; k++) {
			const CellTet<N>* cell = cells.getPtrToCellWithId(id);
			nodePos = cell->n[vmapM[f][k]];
			kNPosTF[pos + k] = getWaveDirection().dot(nodePos) / Constants::c0;
			// Stores minimum kNPos value.
			if (!krminSet) {
				krminSet = true;
				krmin = kNPosTF[pos + k];
			}
			else if (kNPosTF[pos + k] < krmin)
				krmin = kNPosTF[pos + k];
		}
	}
	// Scattered field.
	vector<pair<UInt, UInt> > scatt;
	scatt = getElemFaces(map, cells, scatteredField);
	kNPosSF = new Real[nESF * nfp];
	for (UInt j = 0; j < nESF; j++) {
		UInt id = cells.getIdOfRelPos(scatt[j].first);
		UInt f = scatt[j].second;
		UInt pos = j * nfp;
		for (UInt k = 0; k < nfp; k++) {
			const CellTet<N>* cell = cells.getPtrToCellWithId(id);
			nodePos = cell->n[vmapM[f][k]];
			kNPosSF[pos + k] = getWaveDirection().dot(nodePos) / Constants::c0;
			// Stores minimum kNPos value.
			if (!krminSet) {
				krminSet = true;
				krmin = kNPosSF[pos + k];
			}
			else if (kNPosSF[pos + k] < krmin) {
				krmin = kNPosSF[pos + k];
			}
		}
	}
	// Total field not backed.
	vector<pair<UInt, UInt> > totalNotBacked;
	totalNotBacked = getElemFaces(map, cells, totalFieldNotBacked);
	kNPosTFNB = new Real[nETFNB * nfp];
	for (UInt j = 0; j < nETFNB; j++) {
		UInt id = cells.getIdOfRelPos(totalNotBacked[j].first);
		UInt f = totalNotBacked[j].second;
		UInt pos = j * nfp;
		for (UInt k = 0; k < nfp; k++) {
			const CellTet<N>* cell = cells.getPtrToCellWithId(id);
			nodePos = cell->n[vmapM[f][k]];
			kNPosTFNB[pos + k] = getWaveDirection().dot(nodePos) / Constants::c0;
			// Stores minimum kNPos value.
			if (!krminSet) {
				krminSet = true;
				krmin = kNPosTFNB[pos + k];
			}
			else if (kNPosTFNB[pos + k] < krmin) {
				krmin = kNPosTFNB[pos + k];
			}
		}
	}
	// Syncs minimum.
	krmin = comm->reduceToGlobalMinimum(krmin);
	// Adds krmin to kNPosTSF and kNPosTFNB.
	for (UInt j = 0; j < nETF * nfp; j++) {
		kNPosTF[j] -= krmin;
	}
	for (UInt j = 0; j < nESF * nfp; j++) {
		kNPosSF[j] -= krmin;
	}
	for (UInt j = 0; j < nETFNB * nfp; j++) {
		kNPosTFNB[j] -= krmin;
	}
}
