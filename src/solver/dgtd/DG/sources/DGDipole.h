/*
 * SolverDipole.h
 *
 *  Created on: Oct 3, 2012
 *      Author: luis
 */

#ifndef SOLVERDIPOLE_H_
#define SOLVERDIPOLE_H_

#include "DGSource.h"
#include "math/SphericalVector.h"
#include "sources/Dipole.h"

using namespace std;

//#define SOLVERDIPOLE_DO_NOT_USE_GAUSSIAN_DERIVATIVE
#define SOLVERDIPOLE_CREATE_GRAPH_WITH_EXCITATION

class DGDipole : public DGSource, public Dipole {
public:
    DGDipole(
            const Dipole& dip,
            FieldR3& dE, FieldR3& dH,
            const Int vmapM[faces][nfp]);
    virtual ~DGDipole();
    void computeExcitation(
            const Real intTime,
            const Real minDT);
    CVecR3 getMagnitude(const Real time) const;
    void printInfo() const;
private:
#ifdef SOLVERDIPOLE_DO_NOT_USE_GAUSSIAN_DERIVATIVE
    Real *intT, *intS;
#endif
    SphericalVector *tPos, *sPos;
    void computeExcitationField(
            Real* ExInc, Real* EyInc, Real* EzInc,
            Real* HxInc, Real* HyInc, Real* HzInc,
            const SphericalVector* vPos,
            const UInt nE,
            const Real time) const;
};
#endif /* SOLVERDIPOLE_H_ */
