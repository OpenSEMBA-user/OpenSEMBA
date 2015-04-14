/*
 * SolverPMLUniaxial.cpp
 *
 *  Created on: Aug 2, 2013
 *      Author: luis
 */

#include "DGPMLUniaxial.h"

DGPMLUniaxial::DGPMLUniaxial() {
	J = NULL;
	resJ = NULL;
	rhsJ = NULL;
	M = NULL;
	resM = NULL;
	rhsM = NULL;
}


DGPMLUniaxial::~DGPMLUniaxial() {

}

void
DGPMLUniaxial::addRHSToRes(
 const uint e1, const uint e2,
 const double rka, const double dt) {
	uint i,e;
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(i,e)
	#endif
	for (i = 0; i < dof; i++) {
		e = elem[(i / np) % nElem];
		if (e1 <= e && e < e2) {
			resJ[i] *= rka;
			resJ[i] += rhsJ[i] * dt;
			resM[i] *= rka;
			resM[i] += rhsM[i] * dt;
		}
	}
}

void
DGPMLUniaxial::initUniaxial(
 const PMVolumePML& mat_,
 const CellGroup& cells) {
	init(mat_, cells);
	J = new double [dof];
	resJ = new double [dof];
	rhsJ = new double [dof];
	M = new double [dof];
	resM = new double [dof];
	rhsM = new double [dof];
	for (uint j = 0; j < dof; j++) {
		J[j] = 0.0;
		resJ[j] = 0.0;
		rhsJ[j] = 0.0;
		M[j] = 0.0;
		resM[j] = 0.0;
		rhsM[j] = 0.0;
	}
	assert(check());
}

void
DGPMLUniaxial::internalRHSElectric(
 double* rhsE1, double* rhsE2, double* rhsE3,
 const double* E1, const double* E2, const double* E3,
 const uint e1, const uint e2) const {
	if (useConstantConductivity) {
		uint i, j, e, n;
		#ifdef SOLVER_USE_OPENMP
		#pragma omp parallel for private(i,j,e,n)
		#endif
		for (i = 0; i < dof; i++) {
			e = elem[(i / np) % nElem];
			if (e1 <= e && e < e2) {
				n = i % np;
				j = e * np + n ;
				rhsE1[j] += (eps0*sig) * E1[j] - eps0 * J[i];
				rhsE2[j] -= (eps0*sig) * E2[j];
				rhsE3[j] -= (eps0*sig) * E3[j];
			}
		}
	} else {
		uint i,j,e;
		#ifdef SOLVER_USE_OPENMP
		#pragma omp parallel for private(i,j,e)
		#endif
		for (e = 0; e < nElem; e++) {
			if (e1 <= elem[e] && elem[e] < e2) {
				i = e * np;
				j = elem[e] * np;
				//rhsE1[j] += (eps0*sig1) * E1[j] - eps0 * J[i];
				add_am_v_prod<double,np,np>(&rhsE1[j], sig1[e], &E1[j], eps0);
				sub_a_v_prod<double,np>(&rhsE1[j], &J[i], eps0);
				//rhsE2[j] -= (eps0*sig1) * E2[j];
				sub_am_v_prod<double,np,np>(&rhsE2[j], sig1[e], &E2[j], eps0);
				//rhsE3[j] -= (eps0*sig1) * E3[j];
				sub_am_v_prod<double,np,np>(&rhsE3[j], sig1[e], &E3[j], eps0);
			}
		}
	}
}

void
DGPMLUniaxial::internalRHSMagnetic(
 double* rhsH1, double* rhsH2, double* rhsH3,
 const double* H1, const double* H2, const double* H3,
 const uint e1, const uint e2) const {
	if (useConstantConductivity) {
		uint i, j, e, n;
		#ifdef SOLVER_USE_OPENMP
		#pragma omp parallel for private(i,j,e,n)
		#endif
		for (i = 0; i < dof; i++) {
			e = elem[(i / np) % nElem];
			if (e1 <= e && e < e2) {
				n = i % np;
				j = e * np + n ;
				rhsH1[j] += (mu0*sig) * H1[j] - mu0 * M[i];
				rhsH2[j] -= (mu0*sig) * H2[j];
				rhsH3[j] -= (mu0*sig) * H3[j];
			}
		}
	} else {
		uint i, j, e;
		#ifdef SOLVER_USE_OPENMP
		#pragma omp parallel for private(i,j,e)
		#endif
		for (e = 0; e < nElem; e++) {
			if (e1 <= elem[e] && elem[e] < e2) {
				i = e * np;
				j = elem[e] * np;
				//rhsH1[j] += (mu0*sigma1) * H1[j] - mu0 * M[i];
				add_am_v_prod<double,np,np>(&rhsH1[j], sig1[e], &H1[j], mu0);
				sub_a_v_prod<double,np>(&rhsH1[j], &M[i], mu0);
				//rhsH2[j] -= (mu0*sigma1) * H2[j];
				sub_am_v_prod<double,np,np>(&rhsH2[j], sig1[e], &H2[j], mu0);
				//rhsH3[j] -= (mu0*sigma1) * H3[j];
				sub_am_v_prod<double,np,np>(&rhsH3[j], sig1[e], &H3[j], mu0);
			}
		}
	}
}

void
DGPMLUniaxial::internalRHSElectricPolarizationCurrents(
 const double* E1, const double* E2, const double* E3,
 const uint e1, const uint e2) {
	if (useConstantConductivity) {
		uint i, j, e, n;
		#ifdef SOLVER_USE_OPENMP
		#pragma omp parallel for private(i,j,e,n)
		#endif
		for (i = 0; i < dof; i++) {
			e = elem[(i / np) % nElem];
			if (e1 <= e && e < e2) {
				n = i % np;
				j = e * np + n ;
				rhsJ[i] = E1[j] * (sig*sig) - sig * J[i];
			}
		}
	} else {
		uint i,j,e;
		#ifdef SOLVER_USE_OPENMP
		#pragma omp parallel for private(i,j,e)
		#endif
		for (e = 0; e < nElem; e++) {
			if (e1 <= elem[e] && elem[e] < e2) {
				i = e * np;
				j = elem[e] * np;
				//rhsJ[i] = E1[j] * (sig11) - sig1 * J[i];
				m_v_prod<double,np,np>(&rhsJ[i], sig11[e], &E1[j]);
				sub_m_v_prod<double,np,np>(&rhsJ[i], sig1[e], &J[i]);
			}
		}
	}
}

void
DGPMLUniaxial::internalRHSMagneticPolarizationCurrents(
 const double* H1, const double* H2, const double* H3,
 const uint e1, const uint e2) {
	if (useConstantConductivity) {
		uint i, j, e, n;
		#ifdef SOLVER_USE_OPENMP
		#pragma omp parallel for private(i,j,e,n)
		#endif
		for (i = 0; i < dof; i++) {
			e = elem[(i / np) % nElem];
			if (e1 <= e && e < e2) {
				n = i % np;
				j = e * np + n ;
				rhsM[i] = H1[j] * (sig*sig) - sig * M[i];
			}
		}
	} else {
		uint i, j, e;
		#ifdef SOLVER_USE_OPENMP
		#pragma omp parallel for private(i,j,e)
		#endif
		for (e = 0; e < nElem; e++) {
			if (e1 <= elem[e] && elem[e] < e2) {
				i = e * np;
				j = elem[e] * np;
				//rhsM[i] = H1[j] * (sigma1*sigma1) - sigma1 * M[i];
				m_v_prod<double,np,np>(&rhsM[i], sig11[e], &H1[j]);
				sub_m_v_prod<double,np,np>(&rhsM[i], sig1[e], &M[i]);
			}
		}
	}
}

bool
DGPMLUniaxial::check() const {
	bool sigInitialized = true;
	if (!useConstantConductivity) {
		sigInitialized &= (sig1 != NULL);
		sigInitialized &= (sig2 == NULL);
		sigInitialized &= (sig3 == NULL);
		sigInitialized &= (sig11 != NULL);
		sigInitialized &= (sig22 == NULL);
		sigInitialized &= (sig33 == NULL);
		sigInitialized &= (sig12 == NULL);
		sigInitialized &= (sig23 == NULL);
		sigInitialized &= (sig31 == NULL);
	}
	return sigInitialized;
}

void
DGPMLUniaxial::updateWithRes(
 const uint e1,
 const uint e2,
 const double rkb) {
	uint i, e;
	#ifdef SOLVER_USE_OPENMP
	#pragma omp parallel for private(i, e)
	#endif
	for (i = 0; i < dof; i++) {
		e = elem[(i / np) % nElem];
		if (e1 <= e && e < e2) {
			J[i] += resJ[i] * rkb;
			M[i] += resM[i] * rkb;
		}
	}
}

DGPMLx::DGPMLx(
 const PMVolumePML& mat_,
 const CellGroup& cells,
 const bool useConductivity,
 const double conductivity) {
	useConstantConductivity = useConductivity;
	if (conductivity != 0.0) {
		sig = conductivity;
	}
	initUniaxial(mat_, cells);
}

DGPMLx::~DGPMLx() {

}


void
DGPMLx::computeRHSElectric(Field<double, 3>& rhs,
      const Field<double, 3>& f,
		const uint e1, const uint e2) const {
	internalRHSElectric(rhs.set(x),rhs.set(y),rhs.set(z), f(x),f(y),f(z), e1,e2);
}

void
DGPMLx::computeRHSMagnetic(Field<double, 3>& rhs,
      const Field<double, 3>& f,
		const uint e1, const uint e2) const {
	internalRHSMagnetic(rhs.set(x),rhs.set(y),rhs.set(z), f(x),f(y),f(z), e1,e2);
}

void
DGPMLx::computeRHSElectricPolarizationCurrents(
 const Field<double, 3>& E, const uint e1, const uint e2) {
	internalRHSElectricPolarizationCurrents(E(x),E(y),E(z), e1,e2);
}

void
DGPMLx::computeRHSMagneticPolarizationCurrents(
 const Field<double, 3>& H, const uint e1, const uint e2) {
	internalRHSMagneticPolarizationCurrents(H(x),H(y),H(z), e1,e2);
}

DGPMLy::DGPMLy(
 const PMVolumePML& mat_,
 const CellGroup& cells,
 const bool useConductivity,
 const double conductivity) {
	useConstantConductivity = useConductivity;
	if (conductivity != 0.0) {
		sig = conductivity;
	}
	initUniaxial(mat_, cells);
}

DGPMLy::~DGPMLy() {

}

void
DGPMLy::computeRHSElectric(Field<double, 3>& rhs,
      const Field<double, 3>& f,
		const uint e1, const uint e2) const {
	internalRHSElectric(rhs.set(y),rhs.set(z),rhs.set(x), f(y),f(z),f(x), e1,e2);
}

void
DGPMLy::computeRHSMagnetic(Field<double, 3>& rhs,
      const Field<double, 3>& f,
		const uint e1, const uint e2) const {
	internalRHSMagnetic(rhs.set(y),rhs.set(z),rhs.set(x), f(y),f(z),f(x), e1,e2);
}

void
DGPMLy::computeRHSElectricPolarizationCurrents(
 const Field<double, 3>& f, const uint e1, const uint e2) {
	internalRHSElectricPolarizationCurrents(f(y),f(z),f(x), e1,e2);
}

void
DGPMLy::computeRHSMagneticPolarizationCurrents(
 const Field<double, 3>& f, const uint e1, const uint e2) {
	internalRHSMagneticPolarizationCurrents(f(y),f(z),f(x), e1,e2);
}

DGPMLz::DGPMLz(
 const PMVolumePML& mat_,
 const CellGroup& cells,
 const bool useConductivity,
 const double conductivity) {
	useConstantConductivity = useConductivity;
	if (conductivity != 0.0) {
		sig = conductivity;
	}
	initUniaxial(mat_, cells);
}

DGPMLz::~DGPMLz() {

}


void
DGPMLz::computeRHSElectric(Field<double, 3>& rhs,
      const Field<double, 3>& f,
		const uint e1, const uint e2) const {
	internalRHSElectric(rhs.set(z),rhs.set(x),rhs.set(y), f(z),f(x),f(y), e1,e2);
}

void
DGPMLz::computeRHSMagnetic(Field<double, 3>& rhs,
      const Field<double, 3>& f,
		const uint e1, const uint e2) const {
	internalRHSMagnetic(rhs.set(z),rhs.set(x),rhs.set(y), f(z),f(x),f(y), e1,e2);
}

void
DGPMLz::computeRHSElectricPolarizationCurrents(
 const Field<double, 3>& f, const uint e1, const uint e2) {
	internalRHSElectricPolarizationCurrents(f(z),f(x),f(y), e1,e2);
}

void
DGPMLz::computeRHSMagneticPolarizationCurrents(
 const Field<double, 3>& f, const uint e1, const uint e2) {
	internalRHSMagneticPolarizationCurrents(f(z),f(x),f(y), e1,e2);
}
