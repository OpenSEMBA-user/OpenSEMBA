/*
 * SolverSource.h
 *
 *  Created on: Sep 2, 2013
 *      Author: luis
 */

#ifndef SOLVERSOURCE_H_
#define SOLVERSOURCE_H_

#include <utility>
#include <vector>
#include "../../../../common/inputs/electromagneticSources/EMSource.h"
#include "../../../../common/geometry/maps/Map.h"
#include "../../core/CellGroup.h"
#include "../../core/Comm.h"
#include "../../core/BoundaryCondition.h"

class DGSource {
public:
   typedef enum {
      totalField,
      scatteredField,
      totalFieldNotBacked
   } BackingType;
	DGSource();
	virtual
	~DGSource();
	void
	 addJumps(
	  const uint e1,
	  const uint e2);
	virtual void
	 computeExcitation(
	  const double intTime,
	  const double minDT) = 0;
	virtual void
	 printInfo() const = 0;
protected:
	const static uint N = ORDER_N;
	const static uint np = (N+1) * (N+2) * (N+3) / 6;
	const static uint np2 = np * 2;
	const static uint nfp = (N+1) * (N+2) / 2;
	const static uint npnfp = np * nfp;
	const static uint npnp = np * np;
	const static uint faces = 4;
	const static uint nfpfaces = nfp * faces;
	// Excitation fields.
	double *ExTInc, *EyTInc, *EzTInc;
	double *HxTInc, *HyTInc, *HzTInc;
	double *ExSInc, *EySInc, *EzSInc;
	double *HxSInc, *HySInc, *HzSInc;
	double *ExIncNB, *EyIncNB, *EzIncNB;
	double *HxIncNB, *HyIncNB, *HzIncNB;
	// Excitation total field jumps pointers.
	uint nETF;
	uint *ETFe;
	double **dExT, **dEyT, **dEzT;
	double **dHxT, **dHyT, **dHzT;
	// Excitation scattered field jumps pointers.
	uint nESF;
	uint *ESFe;
	double **dExS, **dEyS, **dEzS;
	double **dHxS, **dHyS, **dHzS;
	// Excitation total field not backed jumps.
	uint nETFNB;
	uint *ETFNBe;
	double **dExTNB, **dEyTNB, **dEzTNB;
	double **dHxTNB, **dHyTNB, **dHzTNB;
	void
	 initSource(
	  const vector<const BoundaryCondition*>& bc,
	  const MapGroup& map,
      const CellGroup& cells,
      FieldR3& dE, FieldR3& dH,
      const int vmapM[faces][nfp]);
	vector<pair<uint,uint> >
	 getElemFaces(
	  const vector<const BoundaryCondition*>& bc,
	  const MapGroup& map,
	  const CellGroup& cells,
	  const BackingType type) const;
	CVecD3*
	 initPositions(
 	  const vector<pair<uint, uint> >& elemFace,
	  const CellGroup& cells) const;
	bool
	 isContainedInPlane(
	  const CartesianPlane plane,
	  const vector<const BoundaryCondition*>& bc) const;
};

#endif /* SOLVERSOURCE_H_ */
