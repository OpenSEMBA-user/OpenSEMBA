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
	  const Field<Real,3>& E,
	  const UInt e1, const UInt e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<Real,3>& H,
	  const UInt e1, const UInt e2);
	void
	 computeRHSElectric(
	  Field<Real,3>& rhsE,
	  const Field<Real,3>& E,
	  const UInt e1, const UInt e2) const;
	void
	 computeRHSMagnetic(
	  Field<Real,3>& rhsE,
	  const Field<Real,3>& E,
	  const UInt e1, const UInt e2) const;
	void
	 addRHSToRes(
	  const UInt e1, const UInt e2,
	  const Real rkb, const Real dt);
	void
	 updateWithRes(
	  const UInt e1,
	  const UInt e2,
	  const Real rkb);
	void
	 addJumps(
	  Field<Real,3>& dE, Field<Real,3>& dH,
	  Field<Real,3>& E, Field<Real,3>& H,
	  const UInt e1, const UInt e2);
	private:
	static const UInt N = ORDER_N;
	static const UInt np = (N+1) * (N+2) * (N+3) / 6;
//	PMVolumeDispersive mat;
	UInt nElem;
	UInt *elem;
	UInt dof, drudeDof;
	// Polarization currents. Size nK x Np x nPoles.
	// Data is stored in nK vectors of Np components for each pole.
	// First nK x Np data correspond to the first pole, and so on.
	Field<complex<Real>,3> P, J;
	Field<complex<Real>,3> rhsP, rhsJ;
	Field<complex<Real>,3> resP, resJ;
private:
	void
	 build(const CellGroup& cells);
};
#endif /* SOLVERDISPERSIVE_H_ */
