/*
 * SolverLeapfrog.h
 *
 *  Created on: Feb 22, 2013
 *      Author: luis
 */

#ifndef INTEGRATORVERLET_H_
#define INTEGRATORVERLET_H_

#include "Integrator.h"

#ifdef LEAPFROG_ORDER
#ifndef SOLVER_IGNORE_DISPERSIVES
	#error Dispersive materials have not been implemented for LF.
#endif
#endif

class IntegratorVerlet : public Integrator {
public:
	IntegratorVerlet();
	virtual ~IntegratorVerlet();
	IntegratorVerlet(
	 const MeshVolume& mesh,
	 const PhysicalModelGroup& pmGroup,
	 const OptionsSolverDGTD* arg);
	void
	 timeIntegrate(
	  const double time) const;
protected:
	uint
 	 getNumOfIterationsPerBigTimeStep(
      const uint e) const;
private:
	static const uint nStages = 2;
	uint
	 getNStages() const;
	double
	 getMaxTimeRatio() const;
	void
	 LTSTimeIntegration(
	  const double localTime,
	  const double localdt,
	  const uint tier) const;
	void
	 updateFieldsVerlet(
	  const uint e1,
	  const uint e2,
	  const double localTime,
	  const double rkdt) const;
};

#endif /* SOLVERLEAPFROG_H_ */
