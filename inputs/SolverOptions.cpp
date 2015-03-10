/*
 * GlobalProblemData.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#include "SolverOptions.h"

SolverOptions::SolverOptions () {
    // Global
    solver_ = Solver::none;
	finalTime_ = 0;
	numberOfTimeSteps_ = 0;
	samplingPeriod_ = 0.0;
	timeStep_ = 0.0;
	cfl_ = 0.0;
	// ugrfdtd
	compositeModel_ = CompositeModel::Default;
    compositesAttenuationFactor_ = 1.0;
    metalModel_ = MetalModel::Default;
    pmlAlpha_ = pair<double,double>(0.0, 1.0);
    pmlKappa_ = 1.0;
    pmlCorrection_ = pair<double,double>(1.0, 1.0);
    wireModel_ = WireModel::Default;
    selfInductanceModel_ = SelfInductanceModel::boutayeb;
    taparrabos_ = false;
    intraWireSimplifications_ = false;
    MTLN_ = false;
    joinWires_ = false;
    groundWires_ = false;
    connectEndings_ = false;
    isolateGroupGroups_ = false;
    makeHoles_ = false;
    wiresAttenuationFactor_ = false;
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

double SolverOptions::getFinalTime() const {
    return finalTime_;
}

void SolverOptions::setFinalTime(double finalTime) {
    finalTime_ = finalTime;
}

double SolverOptions::getSamplingPeriod() const {
    return samplingPeriod_;
}

void SolverOptions::setSamplingPeriod(double samplingPeriod) {
    samplingPeriod_ = samplingPeriod;
}

SolverOptions::Solver SolverOptions::getSolver() const {
    return solver_;
}

void SolverOptions::setSolver(Solver solver) {
    solver_ = solver;
}

double SolverOptions::getTimeStep() const {
    return timeStep_;
}

void SolverOptions::setTimeStep(double timeStep) {
    timeStep_ = timeStep;
}

double SolverOptions::getCfl() const {
    return cfl_;
}

void SolverOptions::setCFL(double cfl) {
    cfl_ = cfl;
}

SolverOptions::CompositeModel SolverOptions::getCompositeModel() const {
    return compositeModel_;
}

void SolverOptions::setCompositeModel(CompositeModel compositeModel) {
    compositeModel_ = compositeModel;
}

double SolverOptions::getCompositesAttenuationFactor() const {
    return compositesAttenuationFactor_;
}

void SolverOptions::setCompositesAttenuationFactor(
        double compositesAttenuationFactor) {
    compositesAttenuationFactor_ = compositesAttenuationFactor;
}

bool SolverOptions::isConnectEndings() const {
    return connectEndings_;
}

void SolverOptions::setConnectEndings(bool connectEndings) {
    connectEndings_ = connectEndings;
}

bool SolverOptions::isGroundWires() const {
    return groundWires_;
}

void SolverOptions::setGroundWires(bool groundWires) {
    groundWires_ = groundWires;
}

bool SolverOptions::isIntraWireSimplifications() const {
    return intraWireSimplifications_;
}

void SolverOptions::setIntraWireSimplifications(bool intraWireSimplifications) {
    intraWireSimplifications_ = intraWireSimplifications;
}

bool SolverOptions::isIsolateGroupGroups() const {
    return isolateGroupGroups_;
}

void SolverOptions::setIsolateGroupGroups(bool isolateGroupGroups) {
    isolateGroupGroups_ = isolateGroupGroups;
}

bool SolverOptions::isJoinWires() const {
    return joinWires_;
}

void SolverOptions::setJoinWires(bool joinWires) {
    joinWires_ = joinWires;
}

bool SolverOptions::isMakeHoles() const {
    return makeHoles_;
}

void SolverOptions::setMakeHoles(bool makeHoles) {
    makeHoles_ = makeHoles;
}

SolverOptions::MetalModel SolverOptions::getMetalModel() const {
    return metalModel_;
}

void SolverOptions::setMetalModel(MetalModel metalModel) {
    metalModel_ = metalModel;
}

bool SolverOptions::isMTLN() const {
    return MTLN_;
}

void SolverOptions::setMTLN(bool mtln) {
    MTLN_ = mtln;
}

uint SolverOptions::getNumberOfTimeSteps() const {
    return numberOfTimeSteps_;
}

void SolverOptions::setNumberOfTimeSteps(uint numberOfTimeSteps) {
    numberOfTimeSteps_ = numberOfTimeSteps;
}

const pair<double, double>& SolverOptions::getPmlAlpha() const {
    return pmlAlpha_;
}

void SolverOptions::setPMLAlpha(const pair<double, double>& pmlAlpha) {
    pmlAlpha_ = pmlAlpha;
}

const pair<double, double>& SolverOptions::getPmlCorrection() const {
    return pmlCorrection_;
}

void SolverOptions::setPMLCorrection(
        const pair<double, double>& pmlCorrection) {
    pmlCorrection_ = pmlCorrection;
}

double SolverOptions::getPmlKappa() const {
    return pmlKappa_;
}

void SolverOptions::setPMLKappa(double pmlKappa) {
    pmlKappa_ = pmlKappa;
}

SolverOptions::SelfInductanceModel
SolverOptions::getSelfInductanceModel() const {
    return selfInductanceModel_;
}

void SolverOptions::setSelfInductanceModel(
        SelfInductanceModel selfInductanceModel) {
    selfInductanceModel_ = selfInductanceModel;
}

bool SolverOptions::isTaparrabos() const {
    return taparrabos_;
}

void SolverOptions::setTaparrabos(bool taparrabos) {
    taparrabos_ = taparrabos;
}

SolverOptions::WireModel SolverOptions::getWireModel() const {
    return wireModel_;
}

void SolverOptions::setWireModel(WireModel wireModel) {
    wireModel_ = wireModel;
}

double SolverOptions::getWiresAttenuationFactor() const {
    return wiresAttenuationFactor_;
}

void SolverOptions::setWiresAttenuationFactor(double wiresAttenuationFactor) {
    wiresAttenuationFactor_ = wiresAttenuationFactor;
}
