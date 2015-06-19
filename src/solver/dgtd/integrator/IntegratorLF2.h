/*
 * SolverLeapfrog.h
 *
 *  Created on: Feb 22, 2013
 *      Author: luis
 */

#ifndef INTEGRATORLF2_H_
#define INTEGRATORLF2_H_

#include "Integrator.h"

#ifdef LEAPFROG_ORDER
#ifndef SOLVER_IGNORE_DISPERSIVES
	#error Dispersive materials have not been implemented for LF.
#endif
#endif

class IntegratorLF2 : public Integrator {
public:
	IntegratorLF2();
	virtual ~IntegratorLF2();
	IntegratorLF2(
	 const MeshVolume& mesh,
	 const PMGroup& pmGroup,
	 const OptionsSolverDGTD* arg);
	void
	 setSolver(DG* solver);
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
	 LTSupdateFieldsElectric(
	  const double localTime,
	  const double localdt,
	  const uint tier) const;
	void
	 LTSupdateFieldsMagnetic(
	  const double localTime,
	  const double localdt,
	  const uint tier) const;
	void
	 updateFields(
	  const uint e1,
	  const uint e2,
	  const double localTime,
	  const double rkdt) const;
	void
	 addRHSToFieldsElectric(
	  const uint e1,
	  const uint e2,
	  const double rkdt) const;
	void
	 addRHSToFieldsMagnetic(
	  const uint e1,
	  const uint e2,
	  const double rkdt) const;
};

#endif /* SOLVERLEAPFROG_H_ */
