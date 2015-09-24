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
	 timeIntegrate(const Real time) const;
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
	 LTSupdateFieldsElectric(
	  const Real localTime,
	  const Real localdt,
	  const UInt tier) const;
	void
	 LTSupdateFieldsMagnetic(
	  const Real localTime,
	  const Real localdt,
	  const UInt tier) const;
	void
	 updateFields(
	  const UInt e1,
	  const UInt e2,
	  const Real localTime,
	  const Real rkdt) const;
	void
	 addRHSToFieldsElectric(
	  const UInt e1,
	  const UInt e2,
	  const Real rkdt) const;
	void
	 addRHSToFieldsMagnetic(
	  const UInt e1,
	  const UInt e2,
	  const Real rkdt) const;
};

#endif /* SOLVERLEAPFROG_H_ */
