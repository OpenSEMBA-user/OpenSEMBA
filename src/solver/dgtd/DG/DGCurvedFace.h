/*
 * SolverCurvedFace.h
 *
 *  Created on: Mar 21, 2013
 *      Author: luis
 */

#ifndef DGCURVEDFACE_H_
#define DGCURVEDFACE_H_

#include "../core/CellTet.h"
#include "../../../common/math/MathMatrix.h"
#include "../../../common/math/Field.h"
#include "SolverMath.h"

// TODO: Curved faces are buggy. An static remanent is left.
// TODO: Observed instability for extreme curvature.

class DGCurvedFace {
public:
	uint solverPosition;
	DGCurvedFace();
	DGCurvedFace(
	 const CellTet<ORDER_N>* cell,
	 const uint f,
	 const uint solverPosition,
	 FieldR3& rhsE_, FieldR3& rhsH_,
	 const FieldR3& dE_, const FieldR3& dH_,
	 const FieldR3& dresE_, const FieldR3& dresH_,
	 const double impP,
	 const double admP,
	 const double impAv,
	 const double admAv);
	virtual ~DGCurvedFace();
	DGCurvedFace&
	 operator=(const DGCurvedFace& rhs);
	void
	 addFluxToRHSElectric(
	  const double upwinding,
	  const bool useResForUpw);
	void
	 addFluxToRHSMagnetic(
	  const double upwinding,
	  const bool useResForUpw);
private:
	static const uint N = ORDER_N;
	static const uint np = (N+1) * (N+2) * (N+3) / 6;
	static const uint nfp = (N+1) * (N+2) / 2;
	double impPAv, admPAv, imp1Av, adm1Av;
	double *rhsEx, *rhsEy, *rhsEz, *rhsHx, *rhsHy, *rhsHz;
	const double *dEx, *dEy, *dEz, *dHx, *dHy, *dHz;
	const double *dresEx, *dresEy, *dresEz, *dresHx, *dresHy, *dresHz;
	double nx[np*nfp], ny[np*nfp], nz[np*nfp];
	double rnx[np*nfp], rny[np*nfp], rnz[np*nfp];
	double cnx[np*nfp], cny[np*nfp], cnz[np*nfp];
};

#endif /* SOLVERCURVEDFACE_H_ */
