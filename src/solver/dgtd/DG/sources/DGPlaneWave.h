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
	 const Int vmapM[faces][nfp]);
	virtual ~DGPlaneWave();
	void
	 computeExcitation(
	  const Real intTime,
	  const Real minDT);
	void
	 printInfo() const;
private:
	Real *kNPosTF; // dim(nETSF*SOLVER_NFP)
	Real *kNPosSF; // dim(nETSF*SOLVER_NFP)
	Real *kNPosTFNB; // dim(nETFNB*SOLVER_NFP)
	void
	 computeExcitationField(
	  Real* ExInc, Real *EyInc, Real *EzInc,
	  Real* HxInc, Real *HyInc, Real *HzInc,
	  const Real* vPos,
	  const UInt nE,
	  const Real intTime);
	void
	initWaveNumberPosition(
	 const MapGroup& map,
	 const CellGroup& cells,
	 const Comm* comm,
	 const Int vmapM[faces][nfp]);
};

#endif /* SOLVERPLANEWAVE_H_ */
