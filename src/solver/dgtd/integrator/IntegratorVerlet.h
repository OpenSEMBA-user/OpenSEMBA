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
	 const PMGroup& pmGroup,
	 const OptionsSolverDGTD* arg);
	void
	 timeIntegrate(
	  const Real time) const;
protected:
	UInt
 	 getNumOfIterationsPerBigTimeStep(
      const UInt e) const;
private:
	static const UInt nStages = 2;
	UInt
	 getNStages() const;
	Real
	 getMaxTimeRatio() const;
	void
	 LTSTimeIntegration(
	  const Real localTime,
	  const Real localdt,
	  const UInt tier) const;
	void
	 updateFieldsVerlet(
	  const UInt e1,
	  const UInt e2,
	  const Real localTime,
	  const Real rkdt) const;
};

#endif /* SOLVERLEAPFROG_H_ */
