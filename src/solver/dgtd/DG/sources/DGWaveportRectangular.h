/*
 * SolverWaveportRectangular.h
 *
 *  Created on: Aug 26, 2013
 *      Author: luis
 */

#ifndef SOLVERWAVEPORTRECTANGULAR_H_
#define SOLVERWAVEPORTRECTANGULAR_H_

#include "DGWaveport.h"

class DGWaveportRectangular : public DGWaveport, public Waveport {
public:
	DGWaveportRectangular(
	 const Waveport& pw,
	 const vector<const BoundaryCondition*>& bc,
	 const MapGroup& map,
	 const CellGroup& cells,
    FieldR3& dE, FieldR3& dH,
	 const int vmapM[faces][nfp]);
	virtual
	~DGWaveportRectangular();
	void
	 computeExcitation(
	  const double intTime,
	  const double minDT);
	void
	 printInfo() const;
private:
	double width, height;
	Waveport::ExcitationMode excitationMode;
	double kcm;
	double intrinsicImpedance;
	double gammaMSum;
	void
	 computeExcitationField(
	  double* ExInc, double *EyInc, double *EzInc,
	  double* HxInc, double *HyInc, double *HzInc,
	  const CVecR3* pos,
	  const uint nE,
	  const double intTime,
	  const double minDT);
};

#endif /* SOLVERWAVEPORTRECTANGULAR_H_ */
