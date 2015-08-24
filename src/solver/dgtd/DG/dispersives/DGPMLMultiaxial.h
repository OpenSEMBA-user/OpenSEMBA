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
    DGPMLMultiaxial(
            const PMVolumePML& mat,
            const bool useConductivity,
            const Real conductivity);
    virtual ~DGPMLMultiaxial();
    void addRHSToRes(
            const UInt e1, const UInt e2,
            const Real rka, const Real dt);
    void updateWithRes(
            const UInt e1,
            const UInt e2,
            const Real rkb);
    virtual void computeRHSElectric(
            FieldR3& rhsE,
            const FieldR3& E,
            const UInt e1, const UInt e2) const = 0;
    virtual void computeRHSMagnetic(
            FieldR3& rhsH,
            const FieldR3& H,
            const UInt e1, const UInt e2) const = 0;
    virtual void computeRHSElectricPolarizationCurrents(
            const FieldR3& E,
            const UInt e1, const UInt e2) = 0;
    virtual void computeRHSMagneticPolarizationCurrents(
            const FieldR3& H,
            const UInt e1, const UInt e2) = 0;
protected:
    FieldR3 J, M, resJ, resM, rhsJ, rhsM;
};

#endif /* SOLVERPMLUNIAXIAL_H_ */
