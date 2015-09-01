/*
 * SolverLeapfrog.h
 *
 *  Created on: Feb 22, 2013
 *      Author: luis
 */

#ifndef INTEGRATORLF2FULL_H_
#define INTEGRATORLF2FULL_H_

#include "Integrator.h"

#ifdef LEAPFROG_ORDER
    #ifndef SOLVER_IGNORE_DISPERSIVES
        #error Dispersive materials have not been implemented for LF.
    #endif
#endif

class IntegratorLF2Full : public Integrator {
public:
    IntegratorLF2Full();
    virtual ~IntegratorLF2Full();
    IntegratorLF2Full(
            const MeshVolume& mesh,
            const PMGroup& pmGroup,
            const OptionsSolverDGTD* arg);
    void
    timeIntegrate(const Real time) const;
protected:
    UInt
    getNumOfIterationsPerBigTimeStep(
            const UInt e) const;
private:
    static const UInt nStages = 2;
    UInt getNStages() const;
    Real getMaxTimeRatio() const;
    void LTSTimeIntegration(
            const Real localTime,
            const Real localdt,
            const UInt tier,
            const UInt stage) const;
    void updateFields(
            const UInt e1,
            const UInt e2,
            const Real localTime,
            const Real rkdt) const;
};

#endif /* SOLVERLEAPFROG_H_ */
