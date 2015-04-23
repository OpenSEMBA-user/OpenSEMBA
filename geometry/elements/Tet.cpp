/*
 * Tet.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef TET_H_
#include "Tet.h"
#endif

Tet::Tet() {

}

Tet::~Tet() {

}

Tri3* Tet::getTri3Face(const UInt f) const {
    const CoordR3* coord[3];
    for (UInt i = 0; i < 3; i++) {
        coord[i] = getSideVertex(f,i);
    }
    return new Tri3(ElementId(0), coord, getLayerId(), getMatId());
}

void Tet::getCubaturePositions(CVecR3 res[SimplexTet<1>::ncp]) const {
    static const UInt ncp = SimplexTet<1>::ncp;
    for (UInt c = 0; c < ncp; c++) {
        for (UInt i = 0; i < 3; i++) {
            res[c](i) = 0.0;
        }
    }
    // Evaluates Lagrange's functions in positions specified by the
    // simplex coordinates of tet.
    Real **lagrEv;
    lagrEv = new Real*[ncp];
    for (UInt i = 0; i < ncp; i++) {
        lagrEv[i] = new Real[numberOfCoordinates()];
    }
    for (UInt c = 0; c < ncp; c++) {
        for (UInt i = 0; i < numberOfCoordinates(); i++) {
            lagrEv[c][i]= getTet().getLagr(i).eval(
            getTet().cubatureCoordinate(c));
        }
    }
    // Computes nodes.
    for (UInt c = 0; c < ncp; c++) {
        for (UInt i = 0; i < numberOfCoordinates(); i++) {
            res[c] += *(getV(i)) * lagrEv[c][i];
        }
    }
    for (UInt i = 0; i < ncp; i++) {
        delete[] lagrEv[i];
    }
    delete[] lagrEv;
}

void Tet::getCubatureJacobianDeterminant(
    Real cJDet[SimplexTet<2>::ncp],
    const StaMatrix<Real,4,4> cJ[SimplexTet<2>::ncp]) const {

    for (UInt c = 0; c < SimplexTet<2>::ncp; c++) {
        cJDet[c] = cJ[c].getDeterminant4x4();
    }
}

void Tet::getCubatureJacobianDeterminant(
    Real cJDet[SimplexTet<2>::ncp]) const {

    StaMatrix<Real,4,4> cJ[SimplexTet<2>::ncp];
    getCubatureJacobian(cJ);
    for (UInt c = 0; c < SimplexTet<2>::ncp; c++) {
        cJDet[c] = cJ[c].getDeterminant4x4();
    }
}

void Tet::getCubatureJacobian(
    StaMatrix<Real,4,4> res[SimplexTet<2>::ncp]) const {

    for (UInt s = 0; s < numberOfFaces(); s++)
        for (UInt i = 0; i < numberOfCoordinates(); i++) {
            const CartesianVector<Real,3> v = *(getV(i));
            for (UInt c = 0; c < SimplexTet<2>::ncp; c++) {
                res[c](0,s) += v(0) * getTet().getCda(i,s,c);
                res[c](1,s) += v(1) * getTet().getCda(i,s,c);
                res[c](2,s) += v(2) * getTet().getCda(i,s,c);
                res[c](3,s) = 1.0;
            }
        }
}

void Tet::getCubatureJacobianHat(
    StaMatrix<Real,4,3> cJHat[SimplexTet<2>::ncp],
    const StaMatrix<Real,4,4> cJ[SimplexTet<2>::ncp],
    const Real cJDet[SimplexTet<2>::ncp]) const {

    // PURPOSE:
    // See chapter 17.3.1 of Filippa's course on Advanced FEM.
    Int ind[3];
    StaMatrix<Real,3,3> Jred, invJred;
    StaMatrix<Real,4,3> res;
    // Main loop, runs over all simplex coordinates.
    for (UInt j = 0; j < numberOfFaces(); j++) {
        // Chooses columns indices that will be substracted from the Jacobian.
        switch (j) {
        case 0:
            ind[0] = 1;	ind[1] = 2;	ind[2] = 3;
            break;
        case 1:
            ind[0] = 0;	ind[1] = 2;	ind[2] = 3;
            break;
        case 2:
            ind[0] = 0;	ind[1] = 1;	ind[2] = 3;
            break;
        case 3:
            ind[0] = 0;	ind[1] = 1;	ind[2] = 2;
            break;
        default:
            cerr << endl << "ERROR @ Tet" << endl;
            cerr << endl << "Invalid face." << endl;
            exit(-1);
        }
        // Builds reduced jacobian for all cubature points.
        for (UInt c = 0; c < SimplexTet<1>::ncp; c++) {
            // Substracts column j to column ind[c], to build J reduced.
            for (UInt k = 0; k < 3; k++) {
                for (UInt i = 0; i < 3; i++) {
                    Jred(k,i) = cJ[c](k,ind[i]) - cJ[c](k,j);
                }
            }
            // Computes reduced jacobian inverse.
            invJred = Jred.invert();
            // Gets Jhat from the summation of reduced jacobians.
            for (UInt m = 0; m < 3; m++) {
                for (UInt n = 0; n < 3; n++) {
                    cJHat[c](j,m) -= invJred(n,m);
                }
            }
        }
    } // Ends j loop, running over faces.
    // Multiplies by Jacobian determinant.
    for (UInt c = 0; c < SimplexTet<1>::ncp; c++) {
        cJHat[c] *= cJDet[c];
    }
}
