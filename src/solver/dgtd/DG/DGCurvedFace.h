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

// TODO: Curved faces are buggy. An static remanent is left.
// TODO: Observed instability for extreme curvature.

class DGCurvedFace {
public:
	UInt solverPosition;
	DGCurvedFace();
	DGCurvedFace(
	 const Cell* cell,
	 const UInt f,
	 const UInt solverPosition,
	 FieldR3& rhsE_, FieldR3& rhsH_,
	 const FieldR3& dE_, const FieldR3& dH_,
	 const FieldR3& dresE_, const FieldR3& dresH_,
	 const Real impP,
	 const Real admP,
	 const Real impAv,
	 const Real admAv);
	virtual ~DGCurvedFace();
	DGCurvedFace&
	 operator=(const DGCurvedFace& rhs);
	void
	 addFluxToRHSElectric(
	  const Real upwinding,
	  const bool useResForUpw);
	void
	 addFluxToRHSMagnetic(
	  const Real upwinding,
	  const bool useResForUpw);
private:
	static const UInt N = ORDER_N;
	static const UInt np = (N+1) * (N+2) * (N+3) / 6;
	static const UInt nfp = (N+1) * (N+2) / 2;
	Real impPAv, admPAv, imp1Av, adm1Av;
	Real *rhsEx, *rhsEy, *rhsEz, *rhsHx, *rhsHy, *rhsHz;
	const Real *dEx, *dEy, *dEz, *dHx, *dHy, *dHz;
	const Real *dresEx, *dresEy, *dresEz, *dresHx, *dresHy, *dresHz;
	Real nx[np*nfp], ny[np*nfp], nz[np*nfp];
	Real rnx[np*nfp], rny[np*nfp], rnz[np*nfp];
	Real cnx[np*nfp], cny[np*nfp], cnz[np*nfp];
};

#endif /* SOLVERCURVEDFACE_H_ */
