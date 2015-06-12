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
    static const uint N = ORDER_N;
    static const uint nfp = (N+1) * (N+2) / 2;
    static const uint np = (N+1) * (N+2) * (N+3) / 6;
    static const uint faces = 4;
public:
    virtual ~DGDispersive() = 0;
    virtual void computeRHSElectric(
            Field<double,3>& rhsE,
            const Field<double,3>& E,
            const uint e1, const uint e2) const = 0;
    virtual void computeRHSMagnetic(
            Field<double,3>& rhsH,
            const Field<double,3>& H,
            const uint e1, const uint e2) const= 0;
    virtual void computeRHSElectricPolarizationCurrents(
            const Field<double,3>& E,
            const uint e1, const uint e2) = 0;
    virtual void computeRHSMagneticPolarizationCurrents(
            const Field<double,3>& H,
            const uint e1, const uint e2) = 0;
    virtual void addRHSToRes(
            const uint e1, const uint e2,
            const double rka, const double dt) = 0;
    virtual void updateWithRes(
            const uint e1,
            const uint e2,
            const double rkb) = 0;
    virtual void addJumps(
            Field<double,3>& dE, Field<double,3>& dH,
            Field<double,3>& E, Field<double,3>& H,
            const uint e1, const uint e2) = 0;
};

#endif /* SOLVERDISPERSIVE_H_ */
