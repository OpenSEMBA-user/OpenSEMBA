/*
 * SolverLeapfrog.cpp
 *
 *  Created on: Feb 22, 2013
 *      Author: luis
 */

#include "IntegratorLF2.h"

IntegratorLF2::IntegratorLF2() {
}

IntegratorLF2::IntegratorLF2(
 const MeshVolume& mesh,
 const PMGroup& pmGroup,
 const OptionsSolverDGTD* arg) {
	timeStepSize = arg->getTimeStepSize();
	timeStepSize *= 0.75;
	if (arg->getUpwinding() > 0.0) {
		timeStepSize *= 0.5;
	}
	if (!arg->isNoLTS()) {
		timeStepSize *= 0.95;
	}
	init(mesh, pmGroup, arg);
}

IntegratorLF2::~IntegratorLF2() {

}

void
IntegratorLF2::setSolver(DG* solver_) {
	solver = solver_;
}

void
IntegratorLF2::timeIntegrate(
 const Real time) const {
	assert(solver!=NULL);
	Real dt = getMaxDT();
	if (doLTS) {
		LTSupdateFieldsElectric(time,dt,getNTiers()-1);
		LTSupdateFieldsMagnetic(time,dt,getNTiers()-1);
	} else {
		updateFields(0,solver->nK,time,dt);
	}
}

UInt
IntegratorLF2::getNStages() const {
	return nStages;
}

Real
IntegratorLF2::getMaxTimeRatio() const {
	return Real (1.0 / 3.0);
}

UInt
IntegratorLF2::getNumOfIterationsPerBigTimeStep(
 const UInt e) const {
	UInt nTiers = getNTiers();
	UInt nStages = getNStages();
	UInt tier = timeTierList(e,1);
	UInt iter = (nTiers - tier) * nStages;
	return iter;
}

void
IntegratorLF2::LTSupdateFieldsElectric(
 Real lTime,
 Real ldt,
 const UInt tier) const {
	UInt e1 = getRange(tier, 0).first;
	UInt e2 = getRange(tier, 1).second;
	if (tier > 0) {
		LTSupdateFieldsElectric(lTime, ldt/3.0, tier-1);
		LTSupdateFieldsMagnetic(lTime+ldt/3.0,ldt/3.0, tier-1);
		LTSupdateFieldsElectric(lTime+ldt*2.0/3.0, ldt/3.0, tier-1);
	}
	solver->computeRHSElectric(e1, e2, lTime, mindt);
	solver->addRHSToFieldsElectric(e1, e2, ldt);
}

void
IntegratorLF2::LTSupdateFieldsMagnetic(
 Real lTime,
 Real ldt,
 const UInt tier) const {
	UInt fK = getRange(tier, 0).first;
	UInt lK = getRange(tier, 1).second;
	if (tier > 0) {
		LTSupdateFieldsMagnetic(lTime, ldt/3.0, tier-1);
		LTSupdateFieldsElectric(lTime+ldt/3.0,ldt/3.0, tier-1);
		LTSupdateFieldsMagnetic(lTime+ldt*2.0/3.0, ldt/3.0, tier-1);
	}
	solver->computeRHSMagnetic(fK, lK, lTime, mindt);
	solver->addRHSToFieldsMagnetic(fK, lK, ldt);
}

void
IntegratorLF2::updateFields(
 const UInt e1,
 const UInt e2,
 const Real localTime,
 const Real rkdt) const {
	solver->computeRHSElectric(e1, e2, localTime, mindt);
	solver->addRHSToFieldsElectric(e1,e2,rkdt);
	solver->computeRHSMagnetic(e1, e2, localTime, mindt);
	solver->addRHSToFieldsMagnetic(e1,e2,rkdt);
}
