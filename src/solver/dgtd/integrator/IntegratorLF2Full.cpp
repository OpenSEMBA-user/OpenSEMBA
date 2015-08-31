/*
 * SolverLeapfrog.cpp
 *
 *  Created on: Feb 22, 2013
 *      Author: luis
 */

#include "IntegratorLF2Full.h"

IntegratorLF2Full::IntegratorLF2Full() {
}

IntegratorLF2Full::IntegratorLF2Full(
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

IntegratorLF2Full::~IntegratorLF2Full() {

}

UInt
IntegratorLF2Full::getNStages() const {
	return nStages;
}

Real
IntegratorLF2Full::getMaxTimeRatio() const {
	return ((Real)1.0 / (Real) nStages);
}

UInt
IntegratorLF2Full::getNumOfIterationsPerBigTimeStep(const UInt e) const {
	UInt nTiers = getNTiers();
	UInt nStages = getNStages();
	UInt tier = timeTierList(e,1);
	UInt stage = timeTierList(e,2);
	UInt iter = 0;
	if (tier == 0) {
		iter = (nTiers - tier) * nStages;
	} else {
		iter = (nTiers - tier) * (nStages + nStages - (stage+1));
	}
	return iter;
}

void
IntegratorLF2Full::timeIntegrate(
 const Real time) const {
	assert(solver != NULL);
	if (doLTS) {
		LTSTimeIntegration(time, getMaxDT(), getNTiers() - 1, 0);
		LTSTimeIntegration(time, getMaxDT(), getNTiers() - 1, 0);
	} else {
		updateFields(0,solver->nK,time,getMaxDT());
		updateFields(0,solver->nK,time,getMaxDT());
	}
}

void
IntegratorLF2Full::LTSTimeIntegration(
 Real localTime,
 Real localdt,
 const UInt tier,
 const UInt stage) const {
	UInt fK, lK;
	static const bool useResForUpwinding = true;
	const UInt nStages = getNStages();
	// Determines range of cells belonging to this tier and stage.
	if (tier == getNTiers()-1) {
		fK = getRange(tier, 0).first;
		lK = getRange(tier, nStages-1).second;
	} else {
		fK = getRange(tier, 0).first;
		lK = getRange(tier+1, stage).first;
	}
	// ------- Updates RHS in current tier ----------------------------
	solver->computeCurlsInRHSElectric(fK,lK);
	solver->computeCurlsInRHSMagnetic(fK,lK);
	solver->computeJumps(fK,lK,localTime,mindt);
	solver->addFluxesToRHSElectric(fK,lK,useResForUpwinding);
	solver->addFluxesToRHSMagnetic(fK,lK,useResForUpwinding);
	// ------- Recursively calls this function ------------------------
	if (tier > 0) {
		Real lts = localdt/((Real) nStages);
		UInt lSaved = getRange(tier, nStages-2).second;
		solver->LTSSaveFieldsAndResidues(fK,lSaved);
		for (UInt s = 0; s < nStages; s++) {
			LTSTimeIntegration(localTime + ((Real) s)*lts,
			 lts, tier-1, nStages-s-1);
		}
		solver->LTSLoadFieldsAndResidues(fK,lSaved);
	}
	// ------- Updates field and residue data in current tier ---------
	solver->addRHSToResidueElectric(fK,lK,localdt);
	solver->addRHSToResidueMagnetic(fK,lK,localdt);
	solver->copyJumpsToResidueJumps(fK,lK);
	solver->swapResiduesAndFields(fK,lK);
}

void
IntegratorLF2Full::updateFields(
 const UInt e1,
 const UInt e2,
 const Real localTime,
 const Real rkdt) const {
	static const bool useResForUpwinding = true;
	solver->computeCurlsInRHSElectric(e1,e2);
	solver->computeCurlsInRHSMagnetic(e1,e2);
	solver->computeJumps(e1,e2,localTime,mindt);
	solver->addFluxesToRHSElectric(e1,e2,useResForUpwinding);
	solver->addFluxesToRHSMagnetic(e1,e2,useResForUpwinding);
	solver->addRHSToResidueElectric(e1,e2,rkdt);
	solver->addRHSToResidueMagnetic(e1,e2,rkdt);
	solver->copyJumpsToResidueJumps(e1,e2);
	solver->swapResiduesAndFields(e1,e2);
}
