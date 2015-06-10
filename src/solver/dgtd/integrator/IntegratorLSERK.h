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
	 const PhysicalModelGroup& pmGroup,
	 const OptionsSolverDGTD* arg);
	void
	 timeIntegrate(const double time) const;
protected:
	uint
 	 getNumOfIterationsPerBigTimeStep(
      const uint e) const;
private:
	bool useMaxStageSizeForLTS;
	static const uint nStages = 5;
	static const double rka[nStages];
	static const double rkb[nStages];
	static const double rkc[nStages];
	double stageSize[nStages];
	double
	 getMaxStageSize() const;
	double
	 getMaxTimeRatio() const;
	void
	 buildRKConstants();
	void
	 LTSTimeIntegration(
	  const double time,
	  const double localTime,
	  const double localdt,
	  const uint tier) const;
	void
	 updateResiduals(
	  const uint e1,
	  const uint e2,
	  const double rka,
	  const double localTime,
	  const double localdt) const;
	uint
	 getNStages() const;
	double
	 getRKA(const uint s) const;
	double
	 getRKB(const uint s) const;
	double
	 getRKC(const uint s) const;
	double
	 getStageSize(const uint s) const;
};
#endif /* SOLVERLSERK_H_ */
