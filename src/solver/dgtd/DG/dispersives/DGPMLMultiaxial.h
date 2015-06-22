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
	  const UInt e1, const UInt e2,
	  const Real rka, const Real dt);
	void
	 updateWithRes(
	  const UInt e1,
	  const UInt e2,
	  const Real rkb);
	virtual void
	 computeRHSElectric(
	  Field<Real,3>& rhsE,
	  const Field<Real,3>& E,
	  const UInt e1, const UInt e2) const = 0;
	virtual void
	 computeRHSMagnetic(
	  Field<Real,3>& rhsH,
	  const Field<Real,3>& H,
	  const UInt e1, const UInt e2) const = 0;
	virtual void
	 computeRHSElectricPolarizationCurrents(
	  const Field<Real,3>& E,
	  const UInt e1, const UInt e2) = 0;
	virtual void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<Real,3>& H,
	  const UInt e1, const UInt e2) = 0;
protected:
	void
	 initMultiaxial(
 	  const PMVolumePML& mat_,
	  const CellGroup& cells);
	void
	 internalBiaxialRHSElectric(
	  Real *rhsE1, Real *rhsE2, Real *rhsE3,
	  const Real *E1, const Real *E2, const Real *E3,
	  const UInt e1, const UInt e2) const;
	void
	 internalBiaxialRHSMagnetic(
	  Real *rhsHx, Real *rhsHy, Real *rhsHz,
	  const Real *Hx, const Real *Hy, const Real *Hz,
	  const UInt e1, const UInt e2) const;
	void
	 internalBiaxialRHSElectricPolarizationCurrent(
	  const Real *E1, const Real *E2, const Real *E3,
	  const UInt e1, const UInt e2);
	void
	 internalBiaxialRHSMagneticPolarizationCurrent(
	  const Real *Hx, const Real *Hy, const Real *Hz,
	  const UInt e1, const UInt e2);
	// Polarization currents. Size nK x Np x nPoles.
	// Data is stored in nK vectors of Np components for each pole.
	// First nK x Np data correspond to the first pole, and so on.
	Real *J1, *J2, *J3;
	Real *M1, *M2, *M3;
	Real *resJ1, *resJ2, *resJ3;
	Real *resM1, *resM2, *resM3;
	Real *rhsJ1, *rhsJ2, *rhsJ3;
	Real *rhsM1, *rhsM2, *rhsM3;
};

class DGPMLxy : public DGPMLMultiaxial {
public:
	DGPMLxy();
	DGPMLxy(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
 	 const Real conductivity);
	virtual ~DGPMLxy();
	void
	 computeRHSElectric(
	  Field<Real,3>& rhs,
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2) const;
	void
	 computeRHSMagnetic(
	  Field<Real,3>& rhs,
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2) const;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2);
};

class DGPMLyz : public DGPMLMultiaxial {
public:
	DGPMLyz();
	DGPMLyz(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
 	 const Real conductivity);
	virtual ~DGPMLyz();
	void
	 computeRHSElectric(
	  Field<Real,3>& rhs,
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2) const;
	void
	 computeRHSMagnetic(
	  Field<Real,3>& rhs,
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2) const;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2);
};

class DGPMLzx : public DGPMLMultiaxial {
public:
	DGPMLzx();
	DGPMLzx(
	 const  PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
 	 const Real conductivity);
	virtual ~DGPMLzx();
	void
	 computeRHSElectric(
	  Field<Real,3>& rhs,
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2) const;
	void
	 computeRHSMagnetic(
	  Field<Real,3>& rhs,
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2) const;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2);
};

class DGPMLxyz: public DGPMLMultiaxial {
public:
	DGPMLxyz();
	DGPMLxyz(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
 	 const Real conductivity);
	virtual ~DGPMLxyz();
	void
	 computeRHSElectric(
	  Field<Real,3>& rhs,
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2) const;
	void
	 computeRHSMagnetic(
	  Field<Real,3>& rhs,
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2) const;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<Real,3>& f,
	  const UInt e1, const UInt e2);
};

#endif /* SOLVERPMLUNIAXIAL_H_ */
