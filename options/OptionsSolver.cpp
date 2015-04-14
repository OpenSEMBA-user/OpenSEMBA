/*
 * GlobalProblemData.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#include "OptionsSolver.h"

OptionsSolver::OptionsSolver () {
    // Global
    solver_ = Solver::none;
    finalTime_ = 0;
    samplingPeriod_ = 0.0;
    timeStep_ = 0.0;
    cfl_ = 0.8;
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
OptionsSolver::printInfo() const {
    cout<< " --- Solver parameters --- " << endl;
    cout<< "Solver: " << toStr(solver_) << endl;
    cout<< "Final time: " << finalTime_ << endl;
    cout<< "Default sampling period: " << samplingPeriod_ << endl;
    cout<< "Time step: " << timeStep_ << endl;
}

void OptionsSolver::set(Arguments& args) {
    // TODO OptionsSolver Set
}

void OptionsSolver::printHelp() const {
    // TODO OptionsSolver printHelp
}

string OptionsSolver::toStr(const Solver solver) const {
    switch (solver) {
    case Solver::ugrfdtd:
        return "ugrfdtd";
    case Solver::cudg3d:
        return "cudg3d";
    default:
        return "none";
    }
}

Real OptionsSolver::getFinalTime() const {
    return finalTime_;
}

void OptionsSolver::setFinalTime(Real finalTime) {
    finalTime_ = finalTime;
}

Real OptionsSolver::getSamplingPeriod() const {
    return samplingPeriod_;
}

void OptionsSolver::setSamplingPeriod(Real samplingPeriod) {
    samplingPeriod_ = samplingPeriod;
}

OptionsSolver::Solver OptionsSolver::getSolver() const {
    return solver_;
}

void OptionsSolver::setSolver(Solver solver) {
    solver_ = solver;
}

Real OptionsSolver::getTimeStep() const {
    return timeStep_;
}

void OptionsSolver::setTimeStep(Real timeStep) {
    timeStep_ = timeStep;
}

double OptionsSolver::getCFL() const {
    return cfl_;
}

void OptionsSolver::setCFL(double cfl) {
    cfl_ = cfl;
}

OptionsSolver::CompositeModel OptionsSolver::getCompositeModel() const {
    return compositeModel_;
}

void OptionsSolver::setCompositeModel(CompositeModel compositeModel) {
    compositeModel_ = compositeModel;
}

double OptionsSolver::getCompositesAttenuationFactor() const {
    return compositesAttenuationFactor_;
}

void OptionsSolver::setCompositesAttenuationFactor(
        double compositesAttenuationFactor) {
    compositesAttenuationFactor_ = compositesAttenuationFactor;
}

bool OptionsSolver::isConnectEndings() const {
    return connectEndings_;
}

void OptionsSolver::setConnectEndings(bool connectEndings) {
    connectEndings_ = connectEndings;
}

bool OptionsSolver::isGroundWires() const {
    return groundWires_;
}

void OptionsSolver::setGroundWires(bool groundWires) {
    groundWires_ = groundWires;
}

bool OptionsSolver::isIntraWireSimplifications() const {
    return intraWireSimplifications_;
}

void OptionsSolver::setIntraWireSimplifications(bool intraWireSimplifications) {
    intraWireSimplifications_ = intraWireSimplifications;
}

bool OptionsSolver::isIsolateGroupGroups() const {
    return isolateGroupGroups_;
}

void OptionsSolver::setIsolateGroupGroups(bool isolateGroupGroups) {
    isolateGroupGroups_ = isolateGroupGroups;
}

bool OptionsSolver::isJoinWires() const {
    return joinWires_;
}

void OptionsSolver::setJoinWires(bool joinWires) {
    joinWires_ = joinWires;
}

bool OptionsSolver::isMakeHoles() const {
    return makeHoles_;
}

void OptionsSolver::setMakeHoles(bool makeHoles) {
    makeHoles_ = makeHoles;
}

OptionsSolver::MetalModel OptionsSolver::getMetalModel() const {
    return metalModel_;
}

void OptionsSolver::setMetalModel(MetalModel metalModel) {
    metalModel_ = metalModel;
}

bool OptionsSolver::isMTLN() const {
    return MTLN_;
}

void OptionsSolver::setMTLN(bool mtln) {
    MTLN_ = mtln;
}

UInt OptionsSolver::getNumberOfTimeSteps() const {
    return (UInt)ceil(finalTime_/timeStep_);
}

void OptionsSolver::setNumberOfTimeSteps(UInt numberOfTimeSteps) {
    finalTime_ = timeStep_*numberOfTimeSteps;
}

const pair<double, double>& OptionsSolver::getPmlAlpha() const {
    return pmlAlpha_;
}

void OptionsSolver::setPMLAlpha(const pair<double, double>& pmlAlpha) {
    pmlAlpha_ = pmlAlpha;
}

const pair<double, double>& OptionsSolver::getPmlCorrection() const {
    return pmlCorrection_;
}

void OptionsSolver::setPMLCorrection(
        const pair<double, double>& pmlCorrection) {
    pmlCorrection_ = pmlCorrection;
}

double OptionsSolver::getPmlKappa() const {
    return pmlKappa_;
}

void OptionsSolver::setPMLKappa(double pmlKappa) {
    pmlKappa_ = pmlKappa;
}

OptionsSolver::SelfInductanceModel
OptionsSolver::getSelfInductanceModel() const {
    return selfInductanceModel_;
}

void OptionsSolver::setSelfInductanceModel(
        SelfInductanceModel selfInductanceModel) {
    selfInductanceModel_ = selfInductanceModel;
}

bool OptionsSolver::isTaparrabos() const {
    return taparrabos_;
}

void OptionsSolver::setTaparrabos(bool taparrabos) {
    taparrabos_ = taparrabos;
}

OptionsSolver::WireModel OptionsSolver::getWireModel() const {
    return wireModel_;
}

void OptionsSolver::setWireModel(WireModel wireModel) {
    wireModel_ = wireModel;
}

double OptionsSolver::getWiresAttenuationFactor() const {
    return wiresAttenuationFactor_;
}

void OptionsSolver::setWiresAttenuationFactor(double wiresAttenuationFactor) {
    wiresAttenuationFactor_ = wiresAttenuationFactor;
}
