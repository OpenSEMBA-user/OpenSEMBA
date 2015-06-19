/*
 * SolverLSERK.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: luis
 */

#include "IntegratorLSERK.h"

#ifndef LSERKINFO_CONSTANTS
#define LSERKINFO_CONSTANTS
	const double IntegratorLSERK::rka[IntegratorLSERK::nStages] = {
			0.0,
			-567301805773.0/1357537059087.0,
			-2404267990393.0/2016746695238.0,
			-3550918686646.0/2091501179385.0,
			-1275806237668.0/842570457699.0};
	const double IntegratorLSERK::rkb[IntegratorLSERK::nStages] = {
			1432997174477.0/9575080441755.0,
			5161836677717.0/13612068292357.0,
			1720146321549.0/2090206949498.0,
			3134564353537.0/4481467310338.0,
			2277821191437.0/14882151754819.0};
	const double IntegratorLSERK::rkc[IntegratorLSERK::nStages] = {
			0.0,
			1432997174477.0/9575080441755.0,
			2526269341429.0/6820363962896.0,
			2006345519317.0/3224310063776.0,
			2802321613138.0/2924317926251.0 };
#endif

IntegratorLSERK::IntegratorLSERK() {
	useMaxStageSizeForLTS = false;
}

IntegratorLSERK::IntegratorLSERK(
 const MeshVolume& mesh,
 const PMGroup& pmGroup,
 const ArgumentsCudg3d* arg) {
	timeStepSize = arg->getTimeStepSize();
	buildRKConstants();
	useMaxStageSizeForLTS = arg->isUseMaxStageSizeForLts();
	init(mesh, pmGroup, arg);
}

IntegratorLSERK::~IntegratorLSERK() {
}

void
IntegratorLSERK::timeIntegrate(const double time) const {
	assert(solver != NULL);
	double dt = getMaxDT();
	if (doLTS) {
		LTSTimeIntegration(time, time, dt, getNTiers() - 1);
	} else  {
		double localTime;
		const uint nStages = getNStages();
		uint nK = solver->nK;
		for (uint s = 0; s < nStages; s++) {
			localTime = time + dt * getRKC(s);
			updateResiduals(0, nK, getRKA(s), localTime, dt);
			solver->updateFieldsWithRes(0, nK, getRKB(s));
		}
	}
}

uint
IntegratorLSERK::getNumOfIterationsPerBigTimeStep(
 const uint e) const {
	uint nTiers = getNTiers();
	uint nStages = getNStages();
	uint tier = timeTierList(e,1);
	uint stage = timeTierList(e,2);
	uint iter = 0;
	if (tier == 0) {
		iter = (nTiers - tier) * nStages;
	} else {
		iter = (nTiers - tier) * (nStages + nStages - (stage+1));
	}
	return iter;
}

uint
IntegratorLSERK::getNStages() const {
	return nStages;
}

double
IntegratorLSERK::getRKA(const uint s) const {
	return rka[s];
}

double
IntegratorLSERK::getRKB(const uint s) const {
	return rkb[s];
}

double
IntegratorLSERK::getRKC(const uint s) const {
	return rkc[s];
}

double
IntegratorLSERK::getStageSize(const uint s) const {
	return stageSize[s];
}

void
IntegratorLSERK::buildRKConstants() {
	for (uint i = 1; i <= nStages; i++) {
		stageSize[i-1] = rkc[i] - rkc[i-1];
	}
	stageSize[nStages - 1] = (double) 1.0 - rkc[nStages-1];
}

double
IntegratorLSERK::getMaxStageSize() const {
	DynMatrix<double> aux(nStages,1);
	for (uint i = 0; i < nStages; i++) {
		aux(i,0) = stageSize[i];
	}
	aux.sortRows(0,0);
	return aux(nStages-1, 0);
}

double
IntegratorLSERK::getMaxTimeRatio() const {
	if (useMaxStageSizeForLTS) {
		return getMaxStageSize();
	} else {
		return ((double)1.0 / (double) nStages);
	}
}

 
void
IntegratorLSERK::LTSTimeIntegration(
 const double time,
 double localTime,
 double localdt,
 const uint tier) const {
	uint fK, lK;
	const uint nStages = getNStages();
	for (uint s = 0; s < nStages; s++) {
		// Determines range of cells belonging to this tier and stage.
		if (tier == getNTiers()-1) {
			fK = getRange(tier, 0).first;
			lK = getRange(tier, nStages-1).second;
		} else {
			fK = getRange(tier, 0).first;
			if (s == nStages-1) {
				lK = getRange(tier, s).second;
			} else {
				lK = getRange(tier+1, 3-s).second;
			}
		}
		// Updates RHS in current tier.
		localTime = time + localdt * getRKC(s);
		double rkdt = localdt * getStageSize(s);
		// Recursively calls this function.
		if (tier > 0) {
			uint lSaved = getRange(tier, nStages-2).second;
			solver->LTSSaveFieldsAndResidues(fK,lSaved);
			LTSTimeIntegration(time, localTime, rkdt, tier-1);
			solver->LTSLoadFieldsAndResidues(fK,lSaved);
		}
		// Updates field and residue data in current tier.
		updateResiduals(fK, lK, getRKA(s), localTime, localdt);
		solver->updateFieldsWithRes(fK,lK, getRKB(s));
	}
}

void
IntegratorLSERK::updateResiduals(
 const uint e1,
 const uint e2,
 const double rka,
 const double localTime,
 const double localdt) const {
	solver->computeRHS(e1,e2,localTime,localdt);
	solver->addRHSToRes(e1,e2,rka,localdt);
}
