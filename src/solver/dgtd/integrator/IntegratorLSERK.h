/*
 * SolverLSERK.h
 *
 *  Created on: Nov 30, 2012
 *      Author: luis
 */
#ifndef INTEGRATORLSERK_H_
#define INTEGRATORLSERK_H_

#include "Integrator.h"

#ifdef LSERK_ORDER
#if LSERK_ORDER != 4
#error LSERK is not implemented for this order.
#endif
#endif

#if SOLVER_ORDER > 4
#warning "SolverLSERK RK order is below the order of polynomial basis."
#endif

class IntegratorLSERK : public Integrator {
    friend class DG;
public:
    IntegratorLSERK();
    virtual ~IntegratorLSERK();
    IntegratorLSERK(
            const MeshVolume& mesh,
            const PMGroup& pmGroup,
            const OptionsSolverDGTD* arg);
    void timeIntegrate(const Real time) const;
protected:
    UInt getNumOfIterationsPerBigTimeStep(
            const UInt e) const;
private:
    bool useMaxStageSizeForLTS;
    static const UInt nStages = 5;
    static const Real rka[nStages];
    static const Real rkb[nStages];
    static const Real rkc[nStages];
    Real stageSize[nStages];
    Real getMaxStageSize() const;
    Real getMaxTimeRatio() const;
    void buildRKConstants();
    void LTSTimeIntegration(
            const Real time,
            const Real localTime,
            const Real localdt,
            const UInt tier) const;
    void updateResiduals(
            const UInt e1,
            const UInt e2,
            const Real rka,
            const Real localTime,
            const Real localdt) const;
    UInt getNStages() const;
    Real getRKA(const UInt s) const;
    Real getRKB(const UInt s) const;
    Real getRKC(const UInt s) const;
    Real getStageSize(const UInt s) const;
};
#endif /* SOLVERLSERK_H_ */
