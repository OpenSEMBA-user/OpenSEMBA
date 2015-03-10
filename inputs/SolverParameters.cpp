/*
 * GlobalProblemData.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#include "SolverParameters.h"

SolverParameters::SolverParameters () {
    solver_ = none;
	finalTime_ = 0;
	samplingPeriod_ = 0.0;
	scalingFactor_ = 1.0;
	timeStep_ = 0.0;
	for (UInt i = 0; i < 3; i++) {
		boundTermination_[i].first = pml;
		boundTermination_[i].second = pml;
	}
}

void
SolverParameters::printInfo() const {
	cout<< " --- Solver parameters --- " << endl;
	cout<< "Solver: " << toStr(solver_) << endl;
	cout<< "Final time: " << finalTime_ << endl;
	cout<< "Geom. scaling factor: " << scalingFactor_ << endl;
	cout<< "Default sampling period: " << samplingPeriod_ << endl;
	cout<< "Time step: " << timeStep_ << endl;
	cout<< "Lower X Bound: " <<	toStr(boundTermination_[0].first) << endl;
	cout<< "Upper X Bound: " <<	toStr(boundTermination_[0].second) << endl;
	cout<< "Lower Y Bound: " << toStr(boundTermination_[1].first) << endl;
	cout<< "Upper Y Bound: " << toStr(boundTermination_[1].second) << endl;
	cout<< "Lower Z Bound: " << toStr(boundTermination_[2].first) << endl;
	cout<< "Upper Z Bound: " << toStr(boundTermination_[2].second) << endl;
	cout<< "Lower boundary padding: " << boundaryPadding_.first << endl;
	cout<< "Upper boundary padding: " << boundaryPadding_.second << endl;
	cout<< "Lower boundary mesh size: " << boundaryMeshSize_.first << endl;
	cout<< "Upper boundary mesh size: " << boundaryMeshSize_.second << endl;
}

string
SolverParameters::toStr(const boundType val) const {
	switch (val) {
	case pec:
		return "PEC";
	case pmc:
		return "PMC";
	case pml:
		return "PML";
	case mur1:
		return "MUR1";
	case mur2:
		return "MUR2";
	case periodic:
		return "Periodic";
	case undefined:
		return "Undefined";
	}
}

void
SolverParameters::applyGeometricScalingFactor(const Real& factor) {
    boundaryPadding_.first *= factor;
    boundaryPadding_.second *= factor;
    boundaryMeshSize_.first *= factor;
    boundaryMeshSize_.second *= factor;
}

string SolverParameters::toStr(const Solver solver) const {
    switch (solver) {
    case ugrfdtd:
        return "ugrfdtd";
    case cudg3d:
        return "cudg3d";
    case none:
        return "none";
    }
}

const pair<CVecR3, CVecR3>& SolverParameters::getBoundaryMeshSize() const {
    return boundaryMeshSize_;
}

void SolverParameters::setBoundaryMeshSize(
        const pair<CVecR3, CVecR3>& boundaryMeshSize) {
    boundaryMeshSize_ = boundaryMeshSize;
}

const pair<CVecR3, CVecR3>& SolverParameters::getBoundaryPadding() const {
    return boundaryPadding_;
}

void SolverParameters::setBoundaryPadding(
        const pair<CVecR3, CVecR3>& boundaryPadding) {
    boundaryPadding_ = boundaryPadding;
}

Real SolverParameters::getFinalTime() const {
    return finalTime_;
}

void SolverParameters::setFinalTime(Real finalTime) {
    finalTime_ = finalTime;
}

Real SolverParameters::getSamplingPeriod() const {
    return samplingPeriod_;
}

void SolverParameters::setSamplingPeriod(Real samplingPeriod) {
    samplingPeriod_ = samplingPeriod;
}

Real SolverParameters::getScalingFactor() const {
    return scalingFactor_;
}

void SolverParameters::setScalingFactor(Real scalingFactor) {
    scalingFactor_ = scalingFactor;
}

SolverParameters::Solver SolverParameters::getSolver() const {
    return solver_;
}

void SolverParameters::setSolver(Solver solver) {
    solver_ = solver;
}

Real SolverParameters::getTimeStep() const {
    return timeStep_;
}

void SolverParameters::setTimeStep(Real timeStep) {
    timeStep_ = timeStep;
}

pair<SolverParameters::boundType, SolverParameters::boundType>
SolverParameters::getBoundTermination(
        const UInt i) const {
    assert(i < 3);
    return boundTermination_[i];
}

void SolverParameters::setBoundTermination(
        const UInt i,
        UInt j,
        boundType bound) {
    assert(i < 3);
    assert(j < 2);
    if (j == 0) {
        boundTermination_[i].first = bound;
    } else {
        boundTermination_[i].second = bound;
    }
}
