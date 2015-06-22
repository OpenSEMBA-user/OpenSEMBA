/*
 * SolverPMLBiaxial.cpp
 *
 *  Created on: Aug 2, 2013
 *      Author: luis
 */

#include "DGPMLMultiaxial.h"

DGPMLMultiaxial::DGPMLMultiaxial() {
    J1 = NULL;
    J2 = NULL;
    J3 = NULL;
    resJ1 = NULL;
    resJ2 = NULL;
    resJ3 = NULL;
    rhsJ1 = NULL;
    rhsJ2 = NULL;
    rhsJ3 = NULL;
    M1 = NULL;
    M2 = NULL;
    M3 = NULL;
    resM1 = NULL;
    resM2 = NULL;
    resM3 = NULL;
    rhsM1 = NULL;
    rhsM2 = NULL;
    rhsM3 = NULL;
}

DGPMLMultiaxial::~DGPMLMultiaxial() {

}

void
DGPMLMultiaxial::addRHSToRes(
        const UInt e1, const UInt e2,
        const Real rka, const Real dt) {
    UInt i,e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,e)
#endif
    for (i = 0; i < dof; i++) {
        e = elem[(i / np) % nElem];
        if (e1 <= e && e < e2) {
            resJ1[i] *= rka;
            resJ1[i] += rhsJ1[i] * dt;
            resJ2[i] *= rka;
            resJ2[i] += rhsJ2[i] * dt;
            resJ3[i] *= rka;
            resJ3[i] += rhsJ3[i] * dt;
            resM1[i] *= rka;
            resM1[i] += rhsM1[i] * dt;
            resM2[i] *= rka;
            resM2[i] += rhsM2[i] * dt;
            resM3[i] *= rka;
            resM3[i] += rhsM3[i] * dt;
        }
    }
}

void
DGPMLMultiaxial::updateWithRes(
        const UInt e1,
        const UInt e2,
        const Real rkb) {
    UInt i, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i, e)
#endif
    for (i = 0; i < dof; i++) {
        e = elem[(i / np) % nElem];
        if (e1 <= e && e < e2) {
            J1[i] += resJ1[i] * rkb;
            J2[i] += resJ2[i] * rkb;
            J3[i] += resJ3[i] * rkb;
            M1[i] += resM1[i] * rkb;
            M2[i] += resM2[i] * rkb;
            M3[i] += resM3[i] * rkb;
        }
    }
}


void
DGPMLMultiaxial::initMultiaxial(
        const PMVolumePML& mat_,
        const CellGroup& cells) {
    init(mat_, cells);
    J1 = new Real [dof];
    J2 = new Real [dof];
    J3 = new Real [dof];
    resJ1 = new Real [dof];
    resJ2 = new Real [dof];
    resJ3 = new Real [dof];
    rhsJ1 = new Real [dof];
    rhsJ2 = new Real [dof];
    rhsJ3 = new Real [dof];
    M1 = new Real [dof];
    M2 = new Real [dof];
    M3 = new Real [dof];
    resM1 = new Real [dof];
    resM2 = new Real [dof];
    resM3 = new Real [dof];
    rhsM1 = new Real [dof];
    rhsM2 = new Real [dof];
    rhsM3 = new Real [dof];
    for (UInt j = 0; j < dof; j++) {
        J1[j] = 0.0;
        J2[j] = 0.0;
        J3[j] = 0.0;
        rhsJ1[j] = 0.0;
        rhsJ2[j] = 0.0;
        rhsJ3[j] = 0.0;
        resJ1[j] = 0.0;
        resJ2[j] = 0.0;
        resJ3[j] = 0.0;
        M1[j] = 0.0;
        M2[j] = 0.0;
        M3[j] = 0.0;
        rhsM1[j] = 0.0;
        rhsM2[j] = 0.0;
        rhsM3[j] = 0.0;
        resM1[j] = 0.0;
        resM2[j] = 0.0;
        resM3[j] = 0.0;
    }
}

void
DGPMLMultiaxial::internalBiaxialRHSElectricPolarizationCurrent(
        const Real* E1, const Real* E2, const Real* E3, const UInt e1,
        const UInt e2) {
    if (useConstantConductivity) {
        UInt i;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i)
#endif
        for (i = 0; i < dof; i++) {
            UInt e = elem[(i / np) % nElem];
            if (e1 <= e && e < e2) {
                rhsJ1[i] = - J1[i]*sig;
                rhsJ2[i] = - J2[i]*sig;
                rhsJ3[i] = J3[i] * (sig*sig);
            }
        }
    } else {
        UInt i, j, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e)
#endif
        for (e = 0; e < nElem; e++) {
            if (e1 <= elem[e] && elem[e] < e2) {
                i = e * np;
                j = elem[e] * np;
                //rhsJ1[i] = + sig11*E1[j] - sig12*E1[j] - sig1*J1[i];
                m_v_prod<Real,np,np>(&rhsJ1[i], sig11[e], &E1[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ1[i], sig12[e], &E1[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ1[i], sig1[e], &J1[i]);
                //rhsJ2[i] = - sig21*E2[j] + sig22*E2[j] - sig2*J2[i];
                m_v_prod<Real,np,np>(&rhsJ2[i], sig22[e], &E2[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ2[i], sig12[e], &E2[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ2[i], sig2[e], &J2[i]);
                //rhsJ3[i] = sig12*J3[i];
                m_v_prod<Real,np,np>(&rhsJ3[i], sig12[e], &J3[i]);
            }
        }
    }
}

void
DGPMLMultiaxial::internalBiaxialRHSMagneticPolarizationCurrent(
        const Real* H1,	const Real* H2, const Real* H3,
        const UInt e1, const UInt e2) {
    if (useConstantConductivity) {
        UInt i;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i)
#endif
        for (i = 0; i < dof; i++) {
            UInt e = elem[(i / np) % nElem];
            if (e1 <= e && e < e2) {
                rhsM1[i] = - M1[i]*sig;
                rhsM2[i] = - M2[i]*sig;
                rhsM3[i] = M3[i] * (sig*sig);
            }
        }
    } else {
        UInt i, j, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e)
#endif
        for (e = 0; e < nElem; e++) {
            if (e1 <= e && e < e2) {
                i = e * np;
                j = elem[e] * np;
                //rhsM1[i] = - sig12*H1[j] + sig11*H1[j] - sig1*M1[i];
                m_v_prod<Real,np,np>(&rhsM1[i], sig11[e], &H1[j]);
                sub_m_v_prod<Real,np,np>(&rhsM1[i], sig12[e], &H1[j]);
                sub_m_v_prod<Real,np,np>(&rhsM1[i], sig1[e], &M1[i]);
                //rhsM2[i] = - sig21*H2[j] + sig22*H2[j] - sig2*M2[i];
                m_v_prod<Real,np,np>(&rhsM2[i], sig22[e], &H2[j]);
                sub_m_v_prod<Real,np,np>(&rhsM2[i], sig12[e], &H2[j]);
                sub_m_v_prod<Real,np,np>(&rhsM2[i], sig2[e], &M2[i]);
                //rhsM3[i] = sig12*M3[i];
                m_v_prod<Real,np,np>(&rhsM3[i], sig12[e], &M3[i]);
            }
        }
    }
}

void
DGPMLMultiaxial::internalBiaxialRHSMagnetic(
        Real* rhsH1, Real* rhsH2, Real* rhsH3, const Real* H1, const Real* H2,
        const Real* H3, const UInt e1, const UInt e2) const {
    if (useConstantConductivity) {
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e,n)
#endif
        UInt i, j, e, n;
        for (i = 0; i < dof; i++) {
            e = elem[(i / np) % nElem];
            if (e1 <= e && e < e2) {
                n = i % np;
                j = e * np + n;
                rhsH1[j] -= M1[i] * Constants::mu0;
                rhsH2[j] -= M2[i] * Constants::mu0;
                rhsH3[j] -= M3[i] * Constants::mu0;
            }
        }
    } else {
        UInt i, j, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e)
#endif
        for (e = 0; e < nElem; e++) {
            if (e1 <= e && e < e2) {
                i = e * np;
                j = elem[e] * np;
                //rhsM3[i] = sig12*M3[i];
                m_v_prod<Real,np,np>(&rhsM3[i], sig12[e], &M3[i]);
                //rhsH1[j] += - H1[j]*Constants::mu0*(sigma2-sigma1) - M1[i]*Constants::mu0;
                sub_am_v_prod<Real,np,np>(&rhsH1[j], sig2[e], &H1[j], Constants::mu0);
                add_am_v_prod<Real,np,np>(&rhsH1[j], sig1[e], &H1[j], Constants::mu0);
                sub_a_v_prod<Real,np>(&rhsH1[j], &M1[i], Constants::mu0);
                //rhsH2[j] += - H2[j]*Constants::mu0*(sigma1-sigma2) - M2[i]*Constants::mu0;
                sub_am_v_prod<Real,np,np>(&rhsH2[j], sig1[e], &H2[j], Constants::mu0);
                add_am_v_prod<Real,np,np>(&rhsH2[j], sig2[e], &H2[j], Constants::mu0);
                sub_a_v_prod<Real,np>(&rhsH2[j], &M2[i], Constants::mu0);
                //rhsH3[j] += - H3[j]*Constants::mu0*(sigma1+sigma2) - M3[i]*Constants::mu0;
                sub_am_v_prod<Real,np,np>(&rhsH3[j], sig1[e], &H3[j], Constants::mu0);
                sub_am_v_prod<Real,np,np>(&rhsH3[j], sig2[e], &H3[j], Constants::mu0);
                sub_a_v_prod<Real,np>(&rhsH3[j], &J3[i], Constants::mu0);
            }
        }
    }
}


void
DGPMLMultiaxial::internalBiaxialRHSElectric(Real* rhsE1,
        Real* rhsE2, Real* rhsE3, const Real* E1, const Real* E2,
        const Real* E3, const UInt e1, const UInt e2) const {
    if (useConstantConductivity) {
        UInt i, j, e, n;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e,n)
#endif
        for (i = 0; i < dof; i++) {
            e = elem[(i / np) % nElem];
            if (e1 <= e && e < e2) {
                n = i % np;
                j = e * np + n;
                rhsE1[j] += - J1[i]*Constants::eps0;
                rhsE2[j] += - J2[i]*Constants::eps0;
                rhsE3[j] += - J3[i]*Constants::eps0;
            }
        }
    } else {
        UInt i, j, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e)
#endif
        for (e = 0; e < nElem; e++) {
            if (e1 <= elem[e] && elem[e] < e2) {
                i = e * np;
                j = elem[e] * np;
                //rhsE1[j] += - sig2*E1[j]*Constants::eps0 + sig1*E1[j]*Constants::eps0 - J1[i]*Constants::eps0;
                sub_am_v_prod<Real,np,np>(&rhsE1[j], sig2[e], &E1[j], Constants::eps0);
                add_am_v_prod<Real,np,np>(&rhsE1[j], sig1[e], &E1[j], Constants::eps0);
                sub_a_v_prod<Real,np>(&rhsE1[j], &J1[i], Constants::eps0);
                //rhsE2[j] += - E2[j]*Constants::eps0*(sig1-sig2) - J2[i]*Constants::eps0;
                sub_am_v_prod<Real,np,np>(&rhsE2[j], sig1[e], &E2[j], Constants::eps0);
                add_am_v_prod<Real,np,np>(&rhsE2[j], sig2[e], &E2[j], Constants::eps0);
                sub_a_v_prod<Real,np>(&rhsE2[j], &J2[i], Constants::eps0);
                //rhsE3[j] += - E3[j]*Constants::eps0*(sig1+sig2) - J3[i]*Constants::eps0;
                sub_am_v_prod<Real,np,np>(&rhsE3[j], sig1[e], &E3[j], Constants::eps0);
                sub_am_v_prod<Real,np,np>(&rhsE3[j], sig2[e], &E3[j], Constants::eps0);
                sub_a_v_prod<Real,np>(&rhsE3[j], &J3[i], Constants::eps0);
            }
        }
    }
}

DGPMLxy::DGPMLxy() {

}

DGPMLxy::DGPMLxy(
        const  PMVolumePML& mat_,
        const CellGroup& cells,
        const bool useConductivity,
        const Real conductivity) {
    useConstantConductivity = useConductivity;
    if (conductivity != 0.0) {
        sig = conductivity;
    }
    initMultiaxial(mat_, cells);
}

DGPMLxy::~DGPMLxy() {

}

void
DGPMLxy::computeRHSElectric(
        FieldR3& rhs, const FieldR3& f,
        const UInt e1, const UInt e2) const {
    internalBiaxialRHSElectric(
            rhs.set(x),rhs.set(y),rhs.set(z), f(x),f(y),f(z), e1,e2);
}

void
DGPMLxy::computeRHSMagnetic(
        FieldR3& rhs, const FieldR3& f,
        const UInt e1, const UInt e2) const {
    internalBiaxialRHSMagnetic(
            rhs.set(x),rhs.set(y),rhs.set(z), f(x),f(y),f(z), e1,e2);
}

void
DGPMLxy::computeRHSElectricPolarizationCurrents(
        const FieldR3& f, const UInt e1, const UInt e2) {
    internalBiaxialRHSElectricPolarizationCurrent(f(x),f(y),f(z), e1,e2);
}

void
DGPMLxy::computeRHSMagneticPolarizationCurrents(
        const FieldR3& f, const UInt e1, const UInt e2) {
    internalBiaxialRHSMagneticPolarizationCurrent(f(x),f(y),f(z), e1,e2);
}

DGPMLyz::DGPMLyz() {

}

DGPMLyz::DGPMLyz(
        const PMVolumePML& mat_,
        const CellGroup& cells,
        const bool useConductivity,
        const Real conductivity) {
    useConstantConductivity = useConductivity;
    if (conductivity != 0.0) {
        sig = conductivity;
    }
    initMultiaxial(mat_, cells);
}

DGPMLyz::~DGPMLyz() {

}

void
DGPMLyz::computeRHSElectric(
        FieldR3& rhs, const FieldR3& f,
        const UInt e1, const UInt e2) const {
    internalBiaxialRHSElectric(
            rhs.set(y),rhs.set(z),rhs.set(x), f(y),f(z),f(x), e1,e2);
}

void
DGPMLyz::computeRHSMagnetic(
        FieldR3& rhs, const FieldR3& f,
        const UInt e1, const UInt e2) const {
    internalBiaxialRHSMagnetic(
            rhs.set(y),rhs.set(z),rhs.set(x), f(y),f(z),f(x), e1,e2);
}

void
DGPMLyz::computeRHSElectricPolarizationCurrents(const FieldR3& f,
        const UInt e1, const UInt e2) {
    internalBiaxialRHSElectricPolarizationCurrent(f(y),f(z),f(x), e1,e2);
}

void
DGPMLyz::computeRHSMagneticPolarizationCurrents(const FieldR3& f,
        const UInt e1, const UInt e2) {
    internalBiaxialRHSMagneticPolarizationCurrent(f(y),f(z),f(x), e1,e2);
}

DGPMLzx::DGPMLzx() {

}

DGPMLzx::DGPMLzx(
        const PMVolumePML& mat_,
        const CellGroup& cells,
        const bool useConductivity,
        const Real conductivity) {
    useConstantConductivity = useConductivity;
    if (conductivity != 0.0) {
        sig = conductivity;
    }
    initMultiaxial(mat_, cells);
}

DGPMLzx::~DGPMLzx() {

}

void
DGPMLzx::computeRHSElectric(
        FieldR3& rhs, const FieldR3& f,
        const UInt e1, const UInt e2) const {
    internalBiaxialRHSElectric(
            rhs.set(z), rhs.set(x), rhs.set(y), f(z),f(x),f(y), e1,e2);
}

void
DGPMLzx::computeRHSMagnetic(
        FieldR3& rhs, const FieldR3& f,
        const UInt e1, const UInt e2) const {
    internalBiaxialRHSMagnetic(
            rhs.set(z), rhs.set(x),rhs.set(y), f(z),f(x),f(y), e1,e2);
}

void
DGPMLzx::computeRHSElectricPolarizationCurrents(
        const FieldR3& f,
        const UInt e1, const UInt e2) {
    internalBiaxialRHSElectricPolarizationCurrent(
            f(z),f(x),f(y), e1,e2);
}

void
DGPMLzx::computeRHSMagneticPolarizationCurrents(
        const FieldR3& f,
        const UInt e1, const UInt e2) {
    internalBiaxialRHSMagneticPolarizationCurrent(
            f(z),f(x),f(y), e1,e2);
}

DGPMLxyz::DGPMLxyz(
        const PMVolumePML& mat_,
        const CellGroup& cells,
        const bool useConductivity,
        const Real conductivity) {
    useConstantConductivity = useConductivity;
    if (conductivity != 0.0) {
        sig = conductivity;
    }
    initMultiaxial(mat_, cells);
}

DGPMLxyz::DGPMLxyz() {
}

DGPMLxyz::~DGPMLxyz() {
}

void
DGPMLxyz::computeRHSElectric(
        FieldR3& rhsE,
        const FieldR3& E,
        const UInt e1, const UInt e2) const {
    if (useConstantConductivity) {
        UInt i, j, e, n;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e,n)
#endif
        for (i = 0; i < dof; i++) {
            e = elem[(i / np) % nElem];
            if (e1 <= e && e < e2) {
                n = i % np;
                j = e * np + n;
                rhsE.set(0)[j] += - E(0)[j]*(Constants::eps0*sig) - J1[i]*Constants::eps0;
                rhsE.set(0)[j] += - E(0)[j]*(Constants::eps0*sig) - J2[i]*Constants::eps0;
                rhsE.set(0)[j] += - E(0)[j]*(Constants::eps0*sig) - J3[i]*Constants::eps0;
            }
        }
    } else {
        UInt i, j, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e)
#endif
        for (e = 0; e < nElem; e++) {
            if (e1 <= elem[e] && e < elem[e]) {
                i = e * np;
                j = elem[e] * np;
                //rhsEx[j] += - Ex[j]*Constants::eps0*(sig3+sig2-sig1) - J1[i]*Constants::eps0;
                sub_am_v_prod<Real,np,np>(&rhsE.set(0)[j], sig3[e], &E(0)[j], Constants::eps0);
                sub_am_v_prod<Real,np,np>(&rhsE.set(0)[j], sig2[e], &E(0)[j], Constants::eps0);
                add_am_v_prod<Real,np,np>(&rhsE.set(0)[j], sig1[e], &E(0)[j], Constants::eps0);
                sub_a_v_prod<Real,np>(&rhsE.set(0)[j], &J1[i], Constants::eps0);
                //rhsEy[j] += - Ey[j]*Constants::eps0*(sig1+sig3-sig2) - J2[i]*Constants::eps0;
                sub_am_v_prod<Real,np,np>(&rhsE.set(1)[j], sig1[e], &E(1)[j], Constants::eps0);
                sub_am_v_prod<Real,np,np>(&rhsE.set(1)[j], sig3[e], &E(1)[j], Constants::eps0);
                add_am_v_prod<Real,np,np>(&rhsE.set(1)[j], sig2[e], &E(1)[j], Constants::eps0);
                sub_a_v_prod<Real,np>(&rhsE.set(1)[j], &J2[i], Constants::eps0);
                //rhsEz[j] += - Ez[j]*Constants::eps0*(sig2+sig1-sig3) - J3[i]*Constants::eps0;
                sub_am_v_prod<Real,np,np>(&rhsE.set(2)[j], sig2[e], &E(2)[j], Constants::eps0);
                sub_am_v_prod<Real,np,np>(&rhsE.set(2)[j], sig1[e], &E(2)[j], Constants::eps0);
                add_am_v_prod<Real,np,np>(&rhsE.set(2)[j], sig3[e], &E(2)[j], Constants::eps0);
                sub_a_v_prod<Real,np>(&rhsE.set(2)[j], &J3[i], Constants::eps0);
            }
        }
    }
}

void
DGPMLxyz::computeRHSMagnetic(
        FieldR3& rhsH,
        const FieldR3& H, const UInt e1, const UInt e2) const {
    if (useConstantConductivity) {
        UInt i, j, e, n;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e,n)
#endif
        for (i = 0; i < dof; i++) {
            e = elem[(i / np) % nElem];
            if (e1 <= e && e < e2) {
                n = i % np;
                j = e * np + n;
                rhsH.set(0)[j] += - H(0)[j]*(Constants::mu0*sig) - M1[i]*Constants::mu0;
                rhsH.set(0)[j] += - H(0)[j]*(Constants::mu0*sig) - M2[i]*Constants::mu0;
                rhsH.set(0)[j] += - H(0)[j]*(Constants::mu0*sig) - M3[i]*Constants::mu0;
            }
        }
    } else {
        UInt i, j, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e)
#endif
        for (e = 0; e < nElem; e++) {
            if (e1 <= elem[e] && elem[e] < e2) {
                i = e * np;
                j = elem[e] * np;
                //rhsHx[j] += - Hx[j]*Constants::mu0*(sig3+sig2-sig1) - M1[i]*Constants::mu0;
                sub_am_v_prod<Real,np,np>(&rhsH.set(0)[j], sig3[e], &H(0)[j], Constants::mu0);
                sub_am_v_prod<Real,np,np>(&rhsH.set(0)[j], sig2[e], &H(0)[j], Constants::mu0);
                add_am_v_prod<Real,np,np>(&rhsH.set(0)[j], sig1[e], &H(0)[j], Constants::mu0);
                sub_a_v_prod<Real,np>(&rhsH.set(0)[j], &M1[i], Constants::mu0);
                //rhsHy[j] += - Hy[j]*Constants::mu0*(sig1+sig3-sig2) - M2[i]*Constants::mu0;
                sub_am_v_prod<Real,np,np>(&rhsH.set(1)[j], sig1[e], &H(1)[j], Constants::mu0);
                sub_am_v_prod<Real,np,np>(&rhsH.set(1)[j], sig3[e], &H(1)[j], Constants::mu0);
                add_am_v_prod<Real,np,np>(&rhsH.set(1)[j], sig2[e], &H(1)[j], Constants::mu0);
                sub_a_v_prod<Real,np>(&rhsH.set(1)[j], &M2[i], Constants::mu0);
                //rhsHz[j] += - Hz[j]*Constants::mu0*(sig2+sig1-sig3) - M3[i]*Constants::mu0;
                sub_am_v_prod<Real,np,np>(&rhsH.set(2)[j], sig2[e], &H(2)[j], Constants::mu0);
                sub_am_v_prod<Real,np,np>(&rhsH.set(2)[j], sig1[e], &H(2)[j], Constants::mu0);
                add_am_v_prod<Real,np,np>(&rhsH.set(2)[j], sig3[e], &H(2)[j], Constants::mu0);
                sub_a_v_prod<Real,np>(&rhsH.set(2)[j], &M3[i], Constants::mu0);
            }
        }
    }
}

void
DGPMLxyz::computeRHSElectricPolarizationCurrents(
        const FieldR3& E, const UInt e1, const UInt e2) {
    if (useConstantConductivity) {
        UInt i;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i)
#endif
        for (i = 0; i < dof; i++) {
            UInt e = elem[(i / np) % nElem];
            if (e1 <= e && e < e2) {
                rhsJ1[i] = - J1[i] * sig;
                rhsJ2[i] = - J2[i] * sig;
                rhsJ3[i] = - J3[i] * sig;
            }
        }
    } else {
        UInt i, j, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e)
#endif
        for (e = 0; e < nElem; e++) {
            if (e1 <= elem[e] && e < elem[e]) {
                i = e * np;
                j = elem[e] * np;
                //rhsJ1[i] = sig23*Ex[j]-sig31*Ex[j]-sig12*Ex[j]+ sig11*Ex[j] - sig1*J1[i];
                m_v_prod<Real,np,np>(&rhsJ1[i], sig23[e], &E(0)[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ1[i], sig31[e], &E(0)[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ1[i], sig12[e], &E(0)[j]);
                add_m_v_prod<Real,np,np>(&rhsJ1[i], sig11[e], &E(0)[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ1[i], sig1[e], &J1[i]);
                //rhsJ2[i] = (sig1-sig2)*(sig3-sig2)*Ey[j] - sig2*J2[i];
                m_v_prod<Real,np,np>(&rhsJ2[i], sig31[e], &E(1)[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ2[i], sig12[e], &E(1)[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ2[i], sig23[e], &E(1)[j]);
                add_m_v_prod<Real,np,np>(&rhsJ2[i], sig22[e], &E(1)[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ2[i], sig2[e], &J2[i]);
                //rhsJ3[i] = (sig2-sig3)*(sig1-sig3)*E(2)[j] - sig3*J3[i];
                m_v_prod<Real,np,np>(&rhsJ3[i], sig12[e], &E(2)[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ3[i], sig23[e], &E(2)[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ3[i], sig31[e], &E(2)[j]);
                add_m_v_prod<Real,np,np>(&rhsJ3[i], sig33[e], &E(2)[j]);
                sub_m_v_prod<Real,np,np>(&rhsJ3[i], sig3[e], &J3[i]);
            }
        }
    }
}

void
DGPMLxyz::computeRHSMagneticPolarizationCurrents(
        const FieldR3& H,
        const UInt e1, const UInt e2) {
    if (useConstantConductivity) {
        UInt i;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i)
#endif
        for (i = 0; i < dof; i++) {
            UInt e = elem[(i / np) % nElem];
            if (e1 <= e && e < e2) {
                rhsM1[i] = - M1[i]*sig;
                rhsM2[i] = - M2[i]*sig;
                rhsM3[i] = - M3[i]*sig;
            }
        }
    } else {
        UInt i, j, e;
#ifdef SOLVER_USE_OPENMP
#pragma omp parallel for private(i,j,e)
#endif
        for (e = 0; e < nElem; e++) {
            if (e1 <= elem[e] && elem[e] < e2) {
                i = e * np;
                j = elem[e] * np;
                //rhsM1[i] = (sig3 - sig1)*(sig2 - sig1)*Hx[j] - sig1*M1[i];
                m_v_prod<Real,np,np>(&rhsM1[i], sig23[e], &H(0)[j]);
                sub_m_v_prod<Real,np,np>(&rhsM1[i], sig31[e], &H(0)[j]);
                sub_m_v_prod<Real,np,np>(&rhsM1[i], sig12[e], &H(0)[j]);
                add_m_v_prod<Real,np,np>(&rhsM1[i], sig11[e], &H(0)[j]);
                sub_m_v_prod<Real,np,np>(&rhsM1[i], sig1[e], &M1[i]);
                //rhsM2[i] = (sig1 - sig2)*(sig3 - sig2)*Hy[j] - sig2*M2[i];
                m_v_prod<Real,np,np>(&rhsM2[i], sig31[e], &H(1)[j]);
                sub_m_v_prod<Real,np,np>(&rhsM2[i], sig12[e], &H(1)[j]);
                sub_m_v_prod<Real,np,np>(&rhsM2[i], sig23[e], &H(1)[j]);
                add_m_v_prod<Real,np,np>(&rhsM2[i], sig22[e], &H(1)[j]);
                sub_m_v_prod<Real,np,np>(&rhsM2[i], sig2[e], &M2[i]);
                //rhsM3[i] = (sig2 - sig3)*(sig1 - sig3)*Hz[j] - sig3*M3[i];
                m_v_prod<Real,np,np>(&rhsM3[i], sig12[e], &H(2)[j]);
                sub_m_v_prod<Real,np,np>(&rhsM3[i], sig23[e], &H(2)[j]);
                sub_m_v_prod<Real,np,np>(&rhsM3[i], sig31[e], &H(2)[j]);
                add_m_v_prod<Real,np,np>(&rhsM3[i], sig33[e], &H(2)[j]);
                sub_m_v_prod<Real,np,np>(&rhsM3[i], sig3[e], &M3[i]);
            }
        }
    }
}
