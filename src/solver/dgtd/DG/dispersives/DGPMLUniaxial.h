/*
 * SolverPMLUniaxial.h
 *
 *  Created on: Aug 2, 2013
 *      Author: luis
 */

#ifndef SOLVERPMLUNIAXIAL_H_
#define SOLVERPMLUNIAXIAL_H_

#include "DGPML.h"

class DGPMLUniaxial : public DGPML {
public:
	DGPMLUniaxial();
	virtual ~DGPMLUniaxial();
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
	 initUniaxial(
	  const PMVolumePML& mat_,
	  const CellGroup& cells);
	void
	 internalRHSElectric(
	  Real* rhsE1, Real* rhsE2, Real* rhsE3,
	  const Real* E1, const Real* E2, const Real* E3,
	  const UInt e1, const UInt e2) const;
	void
	 internalRHSMagnetic(
	  Real* rhsH1, Real* rhsH2, Real* rhsH3,
	  const Real* H1, const Real* H2, const Real* H3,
	  const UInt e1, const UInt e2)const ;
	void
	 internalRHSElectricPolarizationCurrents(
	  const Real* E1, const Real* E2, const Real* E3,
	  const UInt e1, const UInt e2);
	void
	 internalRHSMagneticPolarizationCurrents(
	  const Real* H1, const Real* H2, const Real* H3,
	  const UInt e1, const UInt e2);
	// Polarization currents. Size nK x Np x nPoles.
	// Data is stored in nK vectors of Np components for each pole.
	// First nK x Np data correspond to the first pole, and so on.
	Real *J, *M;
	Real *resJ, *resM;
	Real *rhsJ, *rhsM;
private:
	bool
	 check() const;
};

class DGPMLx : public DGPMLUniaxial {
public:
	DGPMLx();
	DGPMLx(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConstantConductivity_,
	 const Real conductivity);
	virtual ~DGPMLx();
	void
	 computeRHSElectric(
	  Field<Real,3>& rhsE,
	  const Field<Real,3>& E,
	  const UInt e1, const UInt e2) const;
	void
	 computeRHSMagnetic(
	  Field<Real,3>& rhsH,
	  const Field<Real,3>& H,
	  const UInt e1, const UInt e2) const ;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<Real,3>& E,
	  const UInt e1, const UInt e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<Real,3>& H,
	  const UInt e1, const UInt e2);

};

class DGPMLy : public DGPMLUniaxial {
public:
	DGPMLy();
	DGPMLy(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
	 const Real conductivity);
	virtual ~DGPMLy();
	void
	 computeRHSElectric(
	  Field<Real,3>& rhsE,
	  const Field<Real,3>& E,
	  const UInt e1, const UInt e2) const ;
	void
	 computeRHSMagnetic(
	  Field<Real,3>& rhsH,
	  const Field<Real,3>& H,
	  const UInt e1, const UInt e2) const ;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<Real,3>& E,
	  const UInt e1, const UInt e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<Real,3>& H,
	  const UInt e1, const UInt e2);
};

class DGPMLz : public DGPMLUniaxial {
public:
	DGPMLz();
	DGPMLz(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
 	 const Real conductivity);
	virtual ~DGPMLz();
   void
    computeRHSElectric(
     Field<Real,3>& rhsE,
     const Field<Real,3>& E,
     const UInt e1, const UInt e2) const ;
   void
    computeRHSMagnetic(
     Field<Real,3>& rhsH,
     const Field<Real,3>& H,
     const UInt e1, const UInt e2) const ;
   void
    computeRHSElectricPolarizationCurrents(
     const Field<Real,3>& E,
     const UInt e1, const UInt e2);
   void
    computeRHSMagneticPolarizationCurrents(
     const Field<Real,3>& H,
     const UInt e1, const UInt e2);
};



#endif /* SOLVERPMLUNIAXIAL_H_ */
