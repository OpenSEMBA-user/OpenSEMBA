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
	 timeIntegrate(const double time) const;
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
	  const uint tier,
	  const uint stage) const;
	void
	 updateFields(
	  const uint e1,
	  const uint e2,
	  const double localTime,
	  const double rkdt) const;
};

#endif /* SOLVERLEAPFROG_H_ */
