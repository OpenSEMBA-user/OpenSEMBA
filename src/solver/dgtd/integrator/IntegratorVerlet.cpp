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
 const PhysicalModelGroup& pmGroup,
 const ArgumentsCudg3d* arg) {
	timeStepSize = arg->getTimeStepSize();
	timeStepSize *= 0.9;
	if (arg->getUpwinding() > 0.0) {
		timeStepSize *= 0.5;
	}
	init(mesh, pmGroup, arg);
}

IntegratorVerlet::~IntegratorVerlet() {

}

uint
IntegratorVerlet::getNumOfIterationsPerBigTimeStep(
 const uint e) const {
	uint nTiers = getNTiers();
	uint nStages = getNStages();
	uint tier = timeTierList(e,1);
	uint iter = (nTiers - tier) * nStages;
	return iter;
}

uint
IntegratorVerlet::getNStages() const {
	return nStages;
}

double
IntegratorVerlet::getMaxTimeRatio() const {
	return double (0.5);
}

void
IntegratorVerlet::timeIntegrate(
 const double time) const {
	assert(solver != NULL);
	if (doLTS) {
		LTSTimeIntegration(time,getMaxDT(),getNTiers()-1);
	} else {
		updateFieldsVerlet(0,solver->nK,time,getMaxDT());
	}
}

void
IntegratorVerlet::LTSTimeIntegration(
 double localTime,
 double localdt,
 const uint tier) const {
	uint fK = getRange(tier, 0).first;
	uint lK = getRange(tier, 1).second;
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
 const uint e1,
 const uint e2,
 const double localTime,
 const double rkdt) const {
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
