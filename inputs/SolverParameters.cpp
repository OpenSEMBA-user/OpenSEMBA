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
	for (unsigned int i = 0; i < 3; i++) {
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
SolverParameters::applyGeometricScalingFactor(const double& factor) {
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

const pair<CVecD3, CVecD3>& SolverParameters::getBoundaryMeshSize() const {
    return boundaryMeshSize_;
}

void SolverParameters::setBoundaryMeshSize(
        const pair<CVecD3, CVecD3>& boundaryMeshSize) {
    boundaryMeshSize_ = boundaryMeshSize;
}

const pair<CVecD3, CVecD3>& SolverParameters::getBoundaryPadding() const {
    return boundaryPadding_;
}

void SolverParameters::setBoundaryPadding(
        const pair<CVecD3, CVecD3>& boundaryPadding) {
    boundaryPadding_ = boundaryPadding;
}

double SolverParameters::getFinalTime() const {
    return finalTime_;
}

void SolverParameters::setFinalTime(double finalTime) {
    finalTime_ = finalTime;
}

double SolverParameters::getSamplingPeriod() const {
    return samplingPeriod_;
}

void SolverParameters::setSamplingPeriod(double samplingPeriod) {
    samplingPeriod_ = samplingPeriod;
}

double SolverParameters::getScalingFactor() const {
    return scalingFactor_;
}

void SolverParameters::setScalingFactor(double scalingFactor) {
    scalingFactor_ = scalingFactor;
}

Solver SolverParameters::getSolver() const {
    return solver_;
}

void SolverParameters::setSolver(Solver solver) {
    solver_ = solver;
}

double SolverParameters::getTimeStep() const {
    return timeStep_;
}

void SolverParameters::setTimeStep(double timeStep) {
    timeStep_ = timeStep;
}
