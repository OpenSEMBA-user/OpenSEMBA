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
 const int vmapM[faces][nfp]) :
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
 const double intTime, const double minDT) {
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
 double* ExInc, double *EyInc, double *EzInc,
 double* HxInc, double *HyInc, double *HzInc,
 const double* vPos,
 const uint nE,
 const double intTime) {
	// Computes the plane wve excitation corresponding to the face f.
	// The face contins nfp nodes and it is computed for time intTime.
	const uint nFields = nfp*nE;
	for (uint j = 0; j < nFields; j++) {
		double delayedTime = intTime - vPos[j];
		if (delayedTime >= 0) {
			pair<CVecR3,CVecR3> EHInc = getElectromagneticField(delayedTime);
			ExInc[j] = EHInc.first(0);
			EyInc[j] = EHInc.first(1);
			EzInc[j] = EHInc.first(2);
			HxInc[j] = EHInc.second(0);
			HyInc[j] = EHInc.second(1);
			HzInc[j] = EHInc.second(2);
		} else {
			ExInc[j] = (double) 0.0;
			EyInc[j] = (double) 0.0;
			EzInc[j] = (double) 0.0;
			HxInc[j] = (double) 0.0;
			HyInc[j] = (double) 0.0;
			HzInc[j] = (double) 0.0;
		}
	}
}

void
DGPlaneWave::initWaveNumberPosition(
 const MapGroup& map,
 const CellGroup& cells,
 const Comm* comm,
 const int vmapM[faces][nfp]) {
	// Generates kNPosTSF for PW excitation.
	// Computes krmin position.
	double krmin = 0.0;
	bool krminSet = false;
	CartesianVector<double, 3> nodePos;
	// Total field.
	vector<pair<uint, uint> > total;
	total = getElemFaces(bc, map, cells, totalField);
	kNPosTF = new double[nETF * nfp];
	for (uint j = 0; j < nETF; j++) {
		uint id = cells.getIdOfRelPos(total[j].first);
		uint f = total[j].second;
		uint pos = j * nfp;
		for (uint k = 0; k < nfp; k++) {
			const CellTet<N>* cell = cells.getPtrToCellWithId(id);
			nodePos = cell->n[vmapM[f][k]];
			kNPosTF[pos + k] = getWaveDirection().dot(nodePos) / SPEED_OF_LIGHT;
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
	vector<pair<uint, uint> > scatt;
	scatt = getElemFaces(bc, map, cells, scatteredField);
	kNPosSF = new double[nESF * nfp];
	for (uint j = 0; j < nESF; j++) {
		uint id = cells.getIdOfRelPos(scatt[j].first);
		uint f = scatt[j].second;
		uint pos = j * nfp;
		for (uint k = 0; k < nfp; k++) {
			const CellTet<N>* cell = cells.getPtrToCellWithId(id);
			nodePos = cell->n[vmapM[f][k]];
			kNPosSF[pos + k] = getWaveDirection().dot(nodePos) / SPEED_OF_LIGHT;
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
	vector<pair<uint, uint> > totalNotBacked;
	totalNotBacked = getElemFaces(bc, map, cells, totalFieldNotBacked);
	kNPosTFNB = new double[nETFNB * nfp];
	for (uint j = 0; j < nETFNB; j++) {
		uint id = cells.getIdOfRelPos(totalNotBacked[j].first);
		uint f = totalNotBacked[j].second;
		uint pos = j * nfp;
		for (uint k = 0; k < nfp; k++) {
			const CellTet<N>* cell = cells.getPtrToCellWithId(id);
			nodePos = cell->n[vmapM[f][k]];
			kNPosTFNB[pos + k] = getWaveDirection().dot(nodePos) / SPEED_OF_LIGHT;
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
	for (uint j = 0; j < nETF * nfp; j++) {
		kNPosTF[j] -= krmin;
	}
	for (uint j = 0; j < nESF * nfp; j++) {
		kNPosSF[j] -= krmin;
	}
	for (uint j = 0; j < nETFNB * nfp; j++) {
		kNPosTFNB[j] -= krmin;
	}
}
