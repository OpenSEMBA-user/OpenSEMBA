/*
 * SolverDispersive.h
 *
 *  Created on: Sep 11, 2012
 *      Author: luis
 */

#ifndef SOLVERDISPERSIVEVOLUMIC_H_
#define SOLVERDISPERSIVEVOLUMIC_H_

#include "DGDispersive.h"

using namespace std;

class DGDispersiveVolumic : public DGDispersive, public PMVolumeDispersive {
public:
	DGDispersiveVolumic();
	DGDispersiveVolumic(
	 const PMVolumeDispersive&,
	 const CellGroup& cells);
	virtual ~DGDispersiveVolumic();
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<double,3>& E,
	  const uint e1, const uint e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<double,3>& H,
	  const uint e1, const uint e2);
	void
	 computeRHSElectric(
	  Field<double,3>& rhsE,
	  const Field<double,3>& E,
	  const uint e1, const uint e2) const;
	void
	 computeRHSMagnetic(
	  Field<double,3>& rhsE,
	  const Field<double,3>& E,
	  const uint e1, const uint e2) const;
	void
	 addRHSToRes(
	  const uint e1, const uint e2,
	  const double rkb, const double dt);
	void
	 updateWithRes(
	  const uint e1,
	  const uint e2,
	  const double rkb);
	void
	 addJumps(
	  Field<double,3>& dE, Field<double,3>& dH,
	  Field<double,3>& E, Field<double,3>& H,
	  const uint e1, const uint e2);
	private:
	static const uint N = ORDER_N;
	static const uint np = (N+1) * (N+2) * (N+3) / 6;
//	PMVolumeDispersive mat;
	uint nElem;
	uint *elem;
	uint dof, drudeDof;
	// Polarization currents. Size nK x Np x nPoles.
	// Data is stored in nK vectors of Np components for each pole.
	// First nK x Np data correspond to the first pole, and so on.
	Field<complex<double>,3> P, J;
	Field<complex<double>,3> rhsP, rhsJ;
	Field<complex<double>,3> resP, resJ;
private:
	void
	 build(const CellGroup& cells);
};
#endif /* SOLVERDISPERSIVE_H_ */
