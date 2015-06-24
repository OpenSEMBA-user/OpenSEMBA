/*
 * SolverPML.h
 *
 *  Created on: Sep 11, 2012
 *      Author: luis
 */

#ifndef SOLVERPML_H_
#define SOLVERPML_H_

#include "DGDispersive.h"
#include "physicalModel/PMVolumePML.h"

class DGPML : public DGDispersive {
public:
    DGPML(const PMVolumePML& mat);
    virtual ~DGPML();
    void addJumps(
            FieldR3& dE, FieldR3& dH,
            FieldR3& E, FieldR3& H,
            const UInt e1, const UInt e2);
protected:
    UInt dof;
    UInt nElem;
    UInt *elem;
    bool useConstantConductivity;
    static constexpr Real sigDefault = 10e9;
    Real sig;
    static const UInt N = ORDER_N;
    static const UInt np = (N+1) * (N+2) * (N+3) / 6;
    Real **sig1, **sig2, **sig3;
    Real **sig11, **sig22, **sig33;
    Real **sig12, **sig23, **sig31;
private:
    void initConductivityMatrices(
            const PMVolumePML& mat);
    void initConductivity(
            Real **sigma,
            const UInt ori,
            const PMVolumePML& mat);
};

#endif /* SOLVERPML_H_ */
