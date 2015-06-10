/*
 * SolverPMLUniaxial.h
 *
 *  Created on: Aug 2, 2013
 *      Author: luis
 */

#ifndef SOLVERPMLMULTIAXIAL_H_
#define SOLVERPMLMULTIAXIAL_H_

#include "DGPML.h"

class DGPMLMultiaxial : public DGPML {
public:
	DGPMLMultiaxial();
	virtual ~DGPMLMultiaxial();
	void
	 addRHSToRes(
	  const uint e1, const uint e2,
	  const double rka, const double dt);
	void
	 updateWithRes(
	  const uint e1,
	  const uint e2,
	  const double rkb);
	virtual void
	 computeRHSElectric(
	  Field<double,3>& rhsE,
	  const Field<double,3>& E,
	  const uint e1, const uint e2) const = 0;
	virtual void
	 computeRHSMagnetic(
	  Field<double,3>& rhsH,
	  const Field<double,3>& H,
	  const uint e1, const uint e2) const = 0;
	virtual void
	 computeRHSElectricPolarizationCurrents(
	  const Field<double,3>& E,
	  const uint e1, const uint e2) = 0;
	virtual void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<double,3>& H,
	  const uint e1, const uint e2) = 0;
protected:
	void
	 initMultiaxial(
 	  const PMVolumePML& mat_,
	  const CellGroup& cells);
	void
	 internalBiaxialRHSElectric(
	  double *rhsE1, double *rhsE2, double *rhsE3,
	  const double *E1, const double *E2, const double *E3,
	  const uint e1, const uint e2) const;
	void
	 internalBiaxialRHSMagnetic(
	  double *rhsHx, double *rhsHy, double *rhsHz,
	  const double *Hx, const double *Hy, const double *Hz,
	  const uint e1, const uint e2) const;
	void
	 internalBiaxialRHSElectricPolarizationCurrent(
	  const double *E1, const double *E2, const double *E3,
	  const uint e1, const uint e2);
	void
	 internalBiaxialRHSMagneticPolarizationCurrent(
	  const double *Hx, const double *Hy, const double *Hz,
	  const uint e1, const uint e2);
	// Polarization currents. Size nK x Np x nPoles.
	// Data is stored in nK vectors of Np components for each pole.
	// First nK x Np data correspond to the first pole, and so on.
	double *J1, *J2, *J3;
	double *M1, *M2, *M3;
	double *resJ1, *resJ2, *resJ3;
	double *resM1, *resM2, *resM3;
	double *rhsJ1, *rhsJ2, *rhsJ3;
	double *rhsM1, *rhsM2, *rhsM3;
};

class DGPMLxy : public DGPMLMultiaxial {
public:
	DGPMLxy();
	DGPMLxy(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
 	 const double conductivity);
	virtual ~DGPMLxy();
	void
	 computeRHSElectric(
	  Field<double,3>& rhs,
	  const Field<double,3>& f,
	  const uint e1, const uint e2) const;
	void
	 computeRHSMagnetic(
	  Field<double,3>& rhs,
	  const Field<double,3>& f,
	  const uint e1, const uint e2) const;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<double,3>& f,
	  const uint e1, const uint e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<double,3>& f,
	  const uint e1, const uint e2);
};

class DGPMLyz : public DGPMLMultiaxial {
public:
	DGPMLyz();
	DGPMLyz(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
 	 const double conductivity);
	virtual ~DGPMLyz();
	void
	 computeRHSElectric(
	  Field<double,3>& rhs,
	  const Field<double,3>& f,
	  const uint e1, const uint e2) const;
	void
	 computeRHSMagnetic(
	  Field<double,3>& rhs,
	  const Field<double,3>& f,
	  const uint e1, const uint e2) const;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<double,3>& f,
	  const uint e1, const uint e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<double,3>& f,
	  const uint e1, const uint e2);
};

class DGPMLzx : public DGPMLMultiaxial {
public:
	DGPMLzx();
	DGPMLzx(
	 const  PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
 	 const double conductivity);
	virtual ~DGPMLzx();
	void
	 computeRHSElectric(
	  Field<double,3>& rhs,
	  const Field<double,3>& f,
	  const uint e1, const uint e2) const;
	void
	 computeRHSMagnetic(
	  Field<double,3>& rhs,
	  const Field<double,3>& f,
	  const uint e1, const uint e2) const;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<double,3>& f,
	  const uint e1, const uint e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<double,3>& f,
	  const uint e1, const uint e2);
};

class DGPMLxyz: public DGPMLMultiaxial {
public:
	DGPMLxyz();
	DGPMLxyz(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
 	 const double conductivity);
	virtual ~DGPMLxyz();
	void
	 computeRHSElectric(
	  Field<double,3>& rhs,
	  const Field<double,3>& f,
	  const uint e1, const uint e2) const;
	void
	 computeRHSMagnetic(
	  Field<double,3>& rhs,
	  const Field<double,3>& f,
	  const uint e1, const uint e2) const;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<double,3>& f,
	  const uint e1, const uint e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<double,3>& f,
	  const uint e1, const uint e2);
};

#endif /* SOLVERPMLUNIAXIAL_H_ */
