/*
 * SolverPMLUniaxial.h
 *
 *  Created on: Aug 2, 2013
 *      Author: luis
 */

#ifndef SOLVERPMLUNIAXIAL_H_
#define SOLVERPMLUNIAXIAL_H_

#include "DGPML.h"

template<Int D>
class DGPMLUniaxial : public DGPML {
public:
    DGPMLUniaxial(const PMVolumePML& mat, const CellGroup& cells);
    virtual ~DGPMLUniaxial();
    void addRHSToRes(
            const UInt e1, const UInt e2,
            const Real rka, const Real dt);
    void updateWithRes(
            const UInt e1,
            const UInt e2,
            const Real rkb);
    void computeRHSElectric(
            FieldR3& rhsE,
            const FieldR3& E,
            const UInt e1, const UInt e2) const;
    void computeRHSMagnetic(
            FieldR3& rhsH,
            const FieldR3& H,
            const UInt e1, const UInt e2) const;
    void computeRHSElectricPolarizationCurrents(
            const FieldR3& E,
            const UInt e1, const UInt e2);
    void computeRHSMagneticPolarizationCurrents(
            const FieldR3& H,
            const UInt e1, const UInt e2);
protected:
    FieldR1 J, M, resJ, resM, rhsJ, rhsM;
private:
    bool check() const;
    static const CartesianAxis dir1 = CartesianAxis(D);
    static const CartesianAxis dir2 = CartesianAxis((D + 1)%3);
    static const CartesianAxis dir3 = CartesianAxis((D + 2)%3);
};

#include "DGPMLUniaxial.hpp"

typedef DGPMLUniaxial<x> DGPMLx;
typedef DGPMLUniaxial<y> DGPMLy;
typedef DGPMLUniaxial<z> DGPMLz;

#endif /* SOLVERPMLUNIAXIAL_H_ */
