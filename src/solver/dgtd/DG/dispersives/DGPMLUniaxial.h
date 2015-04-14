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
	 initUniaxial(
	  const PMVolumePML& mat_,
	  const CellGroup& cells);
	void
	 internalRHSElectric(
	  double* rhsE1, double* rhsE2, double* rhsE3,
	  const double* E1, const double* E2, const double* E3,
	  const uint e1, const uint e2) const;
	void
	 internalRHSMagnetic(
	  double* rhsH1, double* rhsH2, double* rhsH3,
	  const double* H1, const double* H2, const double* H3,
	  const uint e1, const uint e2)const ;
	void
	 internalRHSElectricPolarizationCurrents(
	  const double* E1, const double* E2, const double* E3,
	  const uint e1, const uint e2);
	void
	 internalRHSMagneticPolarizationCurrents(
	  const double* H1, const double* H2, const double* H3,
	  const uint e1, const uint e2);
	// Polarization currents. Size nK x Np x nPoles.
	// Data is stored in nK vectors of Np components for each pole.
	// First nK x Np data correspond to the first pole, and so on.
	double *J, *M;
	double *resJ, *resM;
	double *rhsJ, *rhsM;
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
	 const double conductivity);
	virtual ~DGPMLx();
	void
	 computeRHSElectric(
	  Field<double,3>& rhsE,
	  const Field<double,3>& E,
	  const uint e1, const uint e2) const;
	void
	 computeRHSMagnetic(
	  Field<double,3>& rhsH,
	  const Field<double,3>& H,
	  const uint e1, const uint e2) const ;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<double,3>& E,
	  const uint e1, const uint e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<double,3>& H,
	  const uint e1, const uint e2);

};

class DGPMLy : public DGPMLUniaxial {
public:
	DGPMLy();
	DGPMLy(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
	 const double conductivity);
	virtual ~DGPMLy();
	void
	 computeRHSElectric(
	  Field<double,3>& rhsE,
	  const Field<double,3>& E,
	  const uint e1, const uint e2) const ;
	void
	 computeRHSMagnetic(
	  Field<double,3>& rhsH,
	  const Field<double,3>& H,
	  const uint e1, const uint e2) const ;
	void
	 computeRHSElectricPolarizationCurrents(
	  const Field<double,3>& E,
	  const uint e1, const uint e2);
	void
	 computeRHSMagneticPolarizationCurrents(
	  const Field<double,3>& H,
	  const uint e1, const uint e2);
};

class DGPMLz : public DGPMLUniaxial {
public:
	DGPMLz();
	DGPMLz(
	 const PMVolumePML& mat_,
	 const CellGroup& cells,
	 const bool useConductivity,
 	 const double conductivity);
	virtual ~DGPMLz();
   void
    computeRHSElectric(
     Field<double,3>& rhsE,
     const Field<double,3>& E,
     const uint e1, const uint e2) const ;
   void
    computeRHSMagnetic(
     Field<double,3>& rhsH,
     const Field<double,3>& H,
     const uint e1, const uint e2) const ;
   void
    computeRHSElectricPolarizationCurrents(
     const Field<double,3>& E,
     const uint e1, const uint e2);
   void
    computeRHSMagneticPolarizationCurrents(
     const Field<double,3>& H,
     const uint e1, const uint e2);
};



#endif /* SOLVERPMLUNIAXIAL_H_ */
