/*
 * SolverPML.h
 *
 *  Created on: Sep 11, 2012
 *      Author: luis
 */

#ifndef SOLVERPML_H_
#define SOLVERPML_H_

#include "DGDispersive.h"

class DGPML : public DGDispersive {
public:
	DGPML();
	virtual ~DGPML();
	void
	 addJumps(
	  Field<double,3>& dE, Field<double,3>& dH,
	  Field<double,3>& E, Field<double,3>& H,
	  const uint e1, const uint e2);
protected:
	uint dof;
	uint nElem;
	uint *elem;
	bool useConstantConductivity;
	static constexpr double sigDefault = 10e9;
	double sig;
	static const uint N = ORDER_N;
	static const uint np = (N+1) * (N+2) * (N+3) / 6;
	double **sig1, **sig2, **sig3;
	double **sig11, **sig22, **sig33;
	double **sig12, **sig23, **sig31;
	void
	 init(
	  const PMVolumePML& mat_,
      const CellGroup& cells);
private:
	void
	 initElemsAndDOFs(
	  const PMVolumePML& mat,
	  const CellGroup& cells);
	void
	 initConductivityMatrices(
	  const PMVolumePML& mat,
	  const CellGroup& cells);
	void
 	 initConductivity(
	  double **sigma,
	  const uint ori,
	  const PMVolumePML& mat,
	  const CellGroup& cells);
};

#endif /* SOLVERPML_H_ */
