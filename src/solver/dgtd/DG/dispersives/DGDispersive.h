/*
 * SolverDispersive.h
 *
 *  Created on: Sep 27, 2014
 *      Author: luis
 */

#ifndef SOLVERDISPERSIVE_H_
#define SOLVERDISPERSIVE_H_

#include <cmath>

using namespace std;

#include "math/Constants.h"
#include "math/Field.h"
#include "physicalModel/PhysicalModel.h"
#include "core/CellGroup.h"
#include "SolverMath.h"

class DGDispersive {
protected:
    static const UInt N = ORDER_N;
    static const UInt nfp = (N+1) * (N+2) / 2;
    static const UInt np = (N+1) * (N+2) * (N+3) / 6;
    static const UInt faces = 4;
public:
    virtual ~DGDispersive() = 0;
    virtual void computeRHSElectric(
            FieldR3& rhsE,
            const FieldR3& E,
            const UInt e1, const UInt e2) const = 0;
    virtual void computeRHSMagnetic(
            FieldR3& rhsH,
            const FieldR3& H,
            const UInt e1, const UInt e2) const= 0;
    virtual void computeRHSElectricPolarizationCurrents(
            const FieldR3& E,
            const UInt e1, const UInt e2) = 0;
    virtual void computeRHSMagneticPolarizationCurrents(
            const FieldR3& H,
            const UInt e1, const UInt e2) = 0;
    virtual void addRHSToRes(
            const UInt e1, const UInt e2,
            const Real rka, const Real dt) = 0;
    virtual void updateWithRes(
            const UInt e1,
            const UInt e2,
            const Real rkb) = 0;
    virtual void addJumps(
            FieldR3& dE, FieldR3& dH,
            FieldR3& E, FieldR3& H,
            const UInt e1, const UInt e2) = 0;
};

#endif /* SOLVERDISPERSIVE_H_ */
