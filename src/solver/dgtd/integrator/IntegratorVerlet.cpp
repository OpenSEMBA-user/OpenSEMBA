/*
 * SolverLeapfrog.cpp
 *
 *  Created on: Feb 22, 2013
 *      Author: luis
 */

#include "IntegratorVerlet.h"

IntegratorVerlet::IntegratorVerlet() {
}

IntegratorVerlet::IntegratorVerlet(
 const MeshVolume& mesh,
 const PMGroup& pmGroup,
 const OptionsSolverDGTD* arg) {
	timeStepSize = arg->getTimeStepSize();
	timeStepSize *= 0.9;
	if (arg->getUpwinding() > 0.0) {
		timeStepSize *= 0.5;
	}
	init(mesh, pmGroup, arg);
}

IntegratorVerlet::~IntegratorVerlet() {

}

UInt
IntegratorVerlet::getNumOfIterationsPerBigTimeStep(
 const UInt e) const {
	UInt nTiers = getNTiers();
	UInt nStages = getNStages();
	UInt tier = timeTierList(e,1);
	UInt iter = (nTiers - tier) * nStages;
	return iter;
}

UInt
IntegratorVerlet::getNStages() const {
	return nStages;
}

Real
IntegratorVerlet::getMaxTimeRatio() const {
	return Real (0.5);
}

void
IntegratorVerlet::timeIntegrate(
 const Real time) const {
	assert(solver != NULL);
	if (doLTS) {
		LTSTimeIntegration(time,getMaxDT(),getNTiers()-1);
	} else {
		updateFieldsVerlet(0,solver->nK,time,getMaxDT());
	}
}

void
IntegratorVerlet::LTSTimeIntegration(
 Real localTime,
 Real localdt,
 const UInt tier) const {
	UInt fK = getRange(tier, 0).first;
	UInt lK = getRange(tier, 1).second;
	if (tier > 0) {
		LTSTimeIntegration(localTime, localdt/2.0, tier-1);
	}
	updateFieldsVerlet(fK,lK,localTime,localdt);
	localTime += localdt / 2.0;
	if (tier > 0) {
		LTSTimeIntegration(localTime, localdt/2.0, tier-1);
	}
}

void
IntegratorVerlet::updateFieldsVerlet(
 const UInt e1,
 const UInt e2,
 const Real localTime,
 const Real rkdt) const {
	solver->computeCurlsInRHSMagnetic(e1,e2);
	solver->computeJumps(e1,e2,localTime,mindt);
	solver->addFluxesToRHSMagnetic(e1,e2);
	solver->addRHSToFieldsMagnetic(e1,e2,rkdt*0.5);
	// ---
	solver->computeCurlsInRHSElectric(e1,e2);
	solver->computeJumps(e1,e2,localTime,mindt);
	solver->addFluxesToRHSElectric(e1,e2);
	solver->addRHSToFieldsElectric(e1,e2,rkdt);
	// ---
	solver->computeCurlsInRHSMagnetic(e1,e2);
	solver->computeJumps(e1,e2,localTime,mindt);
	solver->addFluxesToRHSMagnetic(e1,e2);
	solver->addRHSToFieldsMagnetic(e1,e2,rkdt*0.5);
}
