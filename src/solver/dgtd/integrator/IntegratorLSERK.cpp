// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * SolverLSERK.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: luis
 */

#include "IntegratorLSERK.h"

#ifndef LSERKINFO_CONSTANTS
#define LSERKINFO_CONSTANTS
	const Real IntegratorLSERK::rka[IntegratorLSERK::nStages] = {
			0.0,
			-567301805773.0/1357537059087.0,
			-2404267990393.0/2016746695238.0,
			-3550918686646.0/2091501179385.0,
			-1275806237668.0/842570457699.0};
	const Real IntegratorLSERK::rkb[IntegratorLSERK::nStages] = {
			1432997174477.0/9575080441755.0,
			5161836677717.0/13612068292357.0,
			1720146321549.0/2090206949498.0,
			3134564353537.0/4481467310338.0,
			2277821191437.0/14882151754819.0};
	const Real IntegratorLSERK::rkc[IntegratorLSERK::nStages] = {
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
 const OptionsSolverDGTD* arg) {
	timeStepSize = arg->getTimeStepSize();
	buildRKConstants();
	useMaxStageSizeForLTS = arg->isUseMaxStageSizeForLts();
	init(mesh, pmGroup, arg);
}

IntegratorLSERK::~IntegratorLSERK() {
}

void
IntegratorLSERK::timeIntegrate(const Real time) const {
	assert(solver != NULL);
	Real dt = getMaxDT();
	if (doLTS) {
		LTSTimeIntegration(time, time, dt, getNTiers() - 1);
	} else  {
		Real localTime;
		const UInt nStages = getNStages();
		UInt nK = solver->nK;
		for (UInt s = 0; s < nStages; s++) {
			localTime = time + dt * getRKC(s);
			updateResiduals(0, nK, getRKA(s), localTime, dt);
			solver->updateFieldsWithRes(0, nK, getRKB(s));
		}
	}
}

UInt
IntegratorLSERK::getNumOfIterationsPerBigTimeStep(
 const UInt e) const {
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

UInt
IntegratorLSERK::getNStages() const {
	return nStages;
}

Real
IntegratorLSERK::getRKA(const UInt s) const {
	return rka[s];
}

Real
IntegratorLSERK::getRKB(const UInt s) const {
	return rkb[s];
}

Real
IntegratorLSERK::getRKC(const UInt s) const {
	return rkc[s];
}

Real
IntegratorLSERK::getStageSize(const UInt s) const {
	return stageSize[s];
}

void
IntegratorLSERK::buildRKConstants() {
	for (UInt i = 1; i <= nStages; i++) {
		stageSize[i-1] = rkc[i] - rkc[i-1];
	}
	stageSize[nStages - 1] = (Real) 1.0 - rkc[nStages-1];
}

Real
IntegratorLSERK::getMaxStageSize() const {
	DynMatrix<Real> aux(nStages,1);
	for (UInt i = 0; i < nStages; i++) {
		aux(i,0) = stageSize[i];
	}
	aux.sortRows(0,0);
	return aux(nStages-1, 0);
}

Real
IntegratorLSERK::getMaxTimeRatio() const {
	if (useMaxStageSizeForLTS) {
		return getMaxStageSize();
	} else {
		return ((Real)1.0 / (Real) nStages);
	}
}

 
void
IntegratorLSERK::LTSTimeIntegration(
 const Real time,
 Real localTime,
 Real localdt,
 const UInt tier) const {
	UInt fK, lK;
	const UInt nStages = getNStages();
	for (UInt s = 0; s < nStages; s++) {
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
		Real rkdt = localdt * getStageSize(s);
		// Recursively calls this function.
		if (tier > 0) {
			UInt lSaved = getRange(tier, nStages-2).second;
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
 const UInt e1,
 const UInt e2,
 const Real rka,
 const Real localTime,
 const Real localdt) const {
	solver->computeRHS(e1,e2,localTime,localdt);
	solver->addRHSToRes(e1,e2,rka,localdt);
}
