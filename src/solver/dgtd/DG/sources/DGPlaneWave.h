/*
 * SolverPlaneWave.h
 *
 *  Created on: Sep 11, 2012
 *      Author: luis
 */

#ifndef SOLVERPLANEWAVE_H_
#define SOLVERPLANEWAVE_H_

#include "DGSource.h"

using namespace std;

class DGPlaneWave : public DGSource, public PlaneWave {
public:
	DGPlaneWave(
	 const PlaneWave& pw,
	 const MapGroup& map,
	 const CellGroup& cells,
	 const Comm* comm,
	 FieldR3& dE, FieldR3& dH,
	 const int vmapM[faces][nfp]);
	virtual ~DGPlaneWave();
	void
	 computeExcitation(
	  const double intTime,
	  const double minDT);
	void
	 printInfo() const;
private:
	double *kNPosTF; // dim(nETSF*SOLVER_NFP)
	double *kNPosSF; // dim(nETSF*SOLVER_NFP)
	double *kNPosTFNB; // dim(nETFNB*SOLVER_NFP)
	void
	 computeExcitationField(
	  double* ExInc, double *EyInc, double *EzInc,
	  double* HxInc, double *HyInc, double *HzInc,
	  const double* vPos,
	  const uint nE,
	  const double intTime);
	void
	initWaveNumberPosition(
	 const MapGroup& map,
	 const CellGroup& cells,
	 const Comm* comm,
	 const int vmapM[faces][nfp]);
};

#endif /* SOLVERPLANEWAVE_H_ */
