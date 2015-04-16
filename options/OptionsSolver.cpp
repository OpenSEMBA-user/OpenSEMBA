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

void OptionsSolver::set(const Arguments& arg) {
    if (arg.has("h") || arg.has("help")) {
        printHelp();
    }
    if (arg.has("cfl")) {
        setCFL(atof(arg.get("cfl").c_str()));
    }
    if (arg.has("n")) {
        setNumberOfTimeSteps(atoi(arg.get("n").c_str()));
    }
    if (arg.has("digfilt")) {
        setCompositeModel(CompositeModel::digFilt);
    }
    if (arg.has("mibc")) {
        setCompositeModel(CompositeModel::MIBC);
    }
    if (arg.has("ade")) {
        setCompositeModel(CompositeModel::ADEMIBC);
    }
    if (arg.has("nocompomur")) {
        setCompositeModel(CompositeModel::URMMMT);
    }
    if (arg.has("attc")) {
        setCompositesAttenuationFactor(atof(arg.get("attc").c_str()));
    }
    if (arg.has("pmlalpha")) {
        pair<double,double> factorOrder;
        factorOrder.first = atof(arg.get("pmlalpha",0).c_str());
        factorOrder.second = atof(arg.get("pmlalpha",1).c_str());
        setPMLAlpha(factorOrder);
    }
    if (arg.has("pmlkappa")) {
        setPMLKappa(atof(arg.get("pmlkappa").c_str()));
    }
    if (arg.has("pmlcorr")) {
        pair<double,double> factorDepth;
        factorDepth.first = atof(arg.get("pmlcorr",0).c_str());
        factorDepth.second = atof(arg.get("pmlcorr",1).c_str());
        setPMLCorrection(factorDepth);
    }
    if (arg.has("wiresflavor")) {
        string flavor = arg.get("wiresflavor");
        if (flavor == "transition") {
            setWireModel(WireModel::transition);
        } else if (flavor == "new") {
            setWireModel(WireModel::New);
        } else {
            setWireModel(WireModel::Default);
        }
    }
    if (arg.has("taparrabos")) {
        setTaparrabos(true);
    }
    if (arg.has("intrawiresimplify")) {
        setIntraWireSimplifications(true);
    }
    if (arg.has("mtln")) {
        setMTLN(true);
    }
    if (arg.has("groundwires")) {
        setGroundWires(true);
    }
    if (arg.has("connectendings")) {
        setConnectEndings(true);
    }
    if (arg.has("isolategrougroups")) {
        setIsolateGroupGroups(true);
    }
    if (arg.has("joinwires")) {
        setJoinWires(true);
    }
    if (arg.has("makeholes")) {
        setMakeHoles(true);
    }
    if (arg.has("inductance")) {
        string model = arg.get("inductance");
        if (model == "ledfelt") {
            setSelfInductanceModel(SelfInductanceModel::ledfelt);
        } else if (model == "berenger") {
            setSelfInductanceModel(SelfInductanceModel::berenger);
        } else {
            setSelfInductanceModel(SelfInductanceModel::boutayeb);
        }
    }
    if (arg.has("attw")) {
        setWiresAttenuationFactor(atof(arg.get("attw").c_str()));
    }
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

void
OptionsSolver::printInfo() const {
    cout<< " --- Solver parameters --- " << endl;
    cout<< "Solver: " << toStr(solver_) << endl;
    cout<< "Final time: " << finalTime_ << endl;
    cout<< "Default sampling period: " << samplingPeriod_ << endl;
    cout<< "Time step: " << timeStep_ << endl;
}

string OptionsSolver::toArgsStr() const {
    stringstream ss;
    ss << " -cfl " << getCFL();
    ss << " -n " << getNumberOfTimeSteps();
    ss << " -attc " << getCompositesAttenuationFactor();
    ss << " -attw " << getWiresAttenuationFactor();
    ss << " -pmlalpha " << getPmlAlpha();
    ss << " -pmlkappa " << getPmlKappa();
    ss << " -pmlcorr " << getPmlCorrection();
    ss << toStrIfTrue(" -taparrabos", isTaparrabos()));
    ss << toStrIfTrue(" -intrawiressimplify", isIntraWireSimplifications());
    ss << toStrIfTrue(" -mtln", isMTLN());
    ss << toStrIfTrue(" -joinwires", isJoinWires());
    ss << toStrIfTrue(" -groundwires", isGroundWires());
    ss << toStrIfTrue(" -connectendings", isConnectEndings());
    ss << toStrIfTrue(" -isolategroupgroups", isIsolateGroupGroups());
    ss << toStrIfTrue(" -makeholes", isMakeHoles());
    return ss.str();
}

string
OptionsSolver::toStrIfTrue(const string str, const bool param) const {
    if (param) {
        return str;
    } else {
        return string();
    }
}
