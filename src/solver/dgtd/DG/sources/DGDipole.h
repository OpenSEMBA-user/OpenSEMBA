/*
 * SolverDipole.h
 *
 *  Created on: Oct 3, 2012
 *      Author: luis
 */

#ifndef SOLVERDIPOLE_H_
#define SOLVERDIPOLE_H_

#include "DGSource.h"
#include "../../../../common/math/SphericalVector.h"

using namespace std;

//#define SOLVERDIPOLE_DO_NOT_USE_GAUSSIAN_DERIVATIVE
#define SOLVERDIPOLE_CREATE_GRAPH_WITH_EXCITATION

class DGDipole : public DGSource, public Dipole {
public:
	DGDipole(
	 const Dipole& dip,
	 const vector<const BoundaryCondition*>& bc,
	 const MapGroup& map,
	 const CellGroup& cells,
	 FieldD3& dE, FieldD3& dH,
	 const int vmapM[faces][nfp]);
	virtual ~DGDipole();
	void
	 computeExcitation(
	  const double intTime,
	  const double minDT);
	CVecD3
	 getMagnitude(const double time) const;
	void
	 printInfo() const;
private:
#ifdef SOLVERDIPOLE_DO_NOT_USE_GAUSSIAN_DERIVATIVE
	double *intT, *intS;
#endif
	SphericalVector *tPos, *sPos;
	void
	 computeExcitationField(
	  double* ExInc, double* EyInc, double* EzInc,
	  double* HxInc, double* HyInc, double* HzInc,
	  const SphericalVector* vPos,
	  const uint nE,
	  const double time) const;
};
#endif /* SOLVERDIPOLE_H_ */
