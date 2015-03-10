/*
 * GlobalProblemData.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#include "SolverOptions.h"

SolverOptions::SolverOptions () {
    solver_ = Solver::none;
	finalTime_ = 0;
	samplingPeriod_ = 0.0;
	timeStep_ = 0.0;
}

void
SolverOptions::printInfo() const {
	cout<< " --- Solver parameters --- " << endl;
	cout<< "Solver: " << toStr(solver_) << endl;
	cout<< "Final time: " << finalTime_ << endl;
	cout<< "Default sampling period: " << samplingPeriod_ << endl;
	cout<< "Time step: " << timeStep_ << endl;
}

string SolverOptions::toStr(const Solver solver) const {
    switch (solver) {
    case Solver::ugrfdtd:
        return "ugrfdtd";
    case Solver::cudg3d:
        return "cudg3d";
    default:
        return "none";
    }
}

Real SolverOptions::getFinalTime() const {
    return finalTime_;
}

void SolverOptions::setFinalTime(Real finalTime) {
    finalTime_ = finalTime;
}

Real SolverOptions::getSamplingPeriod() const {
    return samplingPeriod_;
}

void SolverOptions::setSamplingPeriod(Real samplingPeriod) {
    samplingPeriod_ = samplingPeriod;
}

SolverOptions::Solver SolverOptions::getSolver() const {
    return solver_;
}

void SolverOptions::setSolver(Solver solver) {
    solver_ = solver;
}

Real SolverOptions::getTimeStep() const {
    return timeStep_;
}

void SolverOptions::setTimeStep(Real timeStep) {
    timeStep_ = timeStep;
}
