/*
 * SolverPMLBiaxial.cpp
 *
 *  Created on: Aug 2, 2013
 *      Author: luis
 */

#include "DGPMLMultiaxial.h"

DGPMLMultiaxial::DGPMLMultiaxial(
        const PMVolumePML& mat,
        const CellGroup& cells,
        const bool useConductivity,
        const Real conductivity) : DGPML(mat, cells) {
//    J.set(dof, 0.0);
//    resJ.set(dof, 0.0);
//    rhsJ.set(dof, 0.0);
//    M.set(dof, 0.0);
//    resM.set(dof, 0.0);
//    rhsM.set(dof, 0.0);
}

DGPMLMultiaxial::~DGPMLMultiaxial() {

}

void DGPMLMultiaxial::addRHSToRes(
        const UInt e1, const UInt e2,
        const Real rka, const Real dt) {
//    UInt i, e;
//    for (i = 0; i < dof; i++) {
//        e = elem[(i / np) % nElem];
//        if (e1 <= e && e < e2) {
//            resJ[i] *= rka;
//            resJ[i] += rhsJ[i] * dt;
//            resM[i] *= rka;
//            resM[i] += rhsM[i] * dt;
//        }
//    }
}

void DGPMLMultiaxial::updateWithRes(
        const UInt e1,
        const UInt e2,
        const Real rkb) {
//    UInt i, e;
//#ifdef SOLVER_USE_OPENMP
//#pragma omp parallel for private(i, e)
//#endif
//    for (i = 0; i < dof; i++) {
//        e = elem[(i / np) % nElem];
//        if (e1 <= e && e < e2) {
//            J[i] += resJ[i] * rkb;
//            M[i] += resM[i] * rkb;
//        }
//    }
}

