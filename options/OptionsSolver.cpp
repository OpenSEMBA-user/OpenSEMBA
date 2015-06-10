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
    forceRestarting_ = false;
    resumeSimulation_ = false;
    flush_ = 0.0;
    // ugrfdtd
    compositeModel_ = CompositeModel::none;
    MTLN_ = false;
    conformalSkin_ = false;
    noCompoMur_ = false;
    skinDepth_ = false;
    compositesAttenuationFactor_ = 1.0;

    wireModel_ = WireModel::newWireModel;
    inductanceModel_ = InductanceModel::boutayeb;
    newDispersiveFormulation_ = false;
    minDistanceWires_ = 0.5;
    taparrabos_ = false;
    groundWires_ = false;
    makeHoles_ = false;
    wiresAttenuationFactor_ = 1.0;

    useDefaultPML_ = true;
    pmlAlpha_ = pair<double,double>(0.0, 1.0);
    pmlKappa_ = 1.0;
    pmlCorrection_ = pair<double,double>(1.0, 1.0);
    pmlBacking_ = PMLBacking::none;

    map_ = false;
    mapVTK_ = false;
    nF2FFDecimation_ = false;
    noNF2FF_ = NoNF2FF::none;
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
        setCompositeModel(CompositeModel::mibc);
    }
    if (arg.has("ade")) {
        setCompositeModel(CompositeModel::ade);
    }
    if (arg.has("digfilt")) {
        setCompositeModel(CompositeModel::digFilt);
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
            setWireModel(WireModel::transitionWireModel);
        } else if (flavor == "new") {
            setWireModel(WireModel::newWireModel);
        } else if (flavor == "old") {
            setWireModel(WireModel::oldWireModel);
        } else {
            throw Error("Unreckognized wire flavor: " + flavor);
        }
    }
    if (arg.has("taparrabos")) {
        setTaparrabos(true);
    }
    if (arg.has("mtln")) {
        setMTLN(true);
    }
    if (arg.has("groundwires")) {
        setGroundWires(true);
    }
    if (arg.has("makeholes")) {
        setMakeHoles(true);
    }
    if (arg.has("inductance")) {
        string model = arg.get("inductance");
        if (model == "ledfelt") {
            setInductanceModel(InductanceModel::ledfelt);
        } else if (model == "berenger") {
            setInductanceModel(InductanceModel::berenger);
        } else {
            setInductanceModel(InductanceModel::boutayeb);
        }
    }
    if (arg.has("attw")) {
        setWiresAttenuationFactor(atof(arg.get("attw").c_str()));
    }
}

void OptionsSolver::printHelp() const {
    // TODO OptionsSolver printHelp
}

string OptionsSolver::toStr(const Solver solver) {
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

bool OptionsSolver::isGroundWires() const {
    return groundWires_;
}

void OptionsSolver::setGroundWires(bool groundWires) {
    groundWires_ = groundWires;
}

bool OptionsSolver::isMakeHoles() const {
    return makeHoles_;
}

void OptionsSolver::setMakeHoles(bool makeHoles) {
    makeHoles_ = makeHoles;
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
    if (numberOfTimeSteps != 0) {
        finalTime_ = timeStep_ * numberOfTimeSteps;
    }
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

OptionsSolver::InductanceModel
OptionsSolver::getInductanceModel() const {
    return inductanceModel_;
}

void OptionsSolver::setInductanceModel(
        InductanceModel selfInductanceModel) {
    inductanceModel_ = selfInductanceModel;
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
    OptionsSolver defaultOptions;
    stringstream ss;
    // --- Global solvers options.
    if (getCFL() != defaultOptions.getCFL()) {
        ss << " -cfl " << getCFL();
    }
    if (isResumeSimulation()) {
        if (getTimeStep() != 0.0) {
            ss << " -r " << (UInt) floor(getFinalTime() / getTimeStep());
        } else {
            ss << " -r " << (UInt) floor(getFinalTime());
        }
    }
    if (isForceRestarting()) {
        ss << " -s";
    }
    if (getFlush() != defaultOptions.getFlush()) {
        ss << " -flush " << getFlush();
    }
    // --- Composites.
    switch (getCompositeModel()) {
    case CompositeModel::digFilt:
        ss << " -digfilt";
        break;
    case CompositeModel::mibc:
        ss << " -mibc";
        ss << toStrIfTrue(" -conformalskin", isConformalSkin());
        ss << toStrIfTrue(" -nocompomur", isNoCompoMur());
        ss << toStrIfTrue(" -skindepth", isSkinDepth());
        break;
    case CompositeModel::ade:
        ss << " -ade";
        ss << toStrIfTrue(" -conformalskin", isConformalSkin());
        ss << toStrIfTrue(" -nocompomur", isNoCompoMur());
        ss << toStrIfTrue(" -skindepth", isSkinDepth());
        break;
    default:
        break;
    }
    if (getCompositesAttenuationFactor() !=
            defaultOptions.getCompositesAttenuationFactor()) {
        ss << " -attc " << getCompositesAttenuationFactor();
    }
    // --- Wire model.
    switch (getWireModel()) {
    case WireModel::newWireModel:
        ss << toStrIfTrue(" -newdispersive", isNewDispersiveFormulation());
        if (getMinDistanceWires() != defaultOptions.getMinDistanceWires()) {
            ss << " -mindistwires " << getMinDistanceWires();
        }
        break;
    case WireModel::transitionWireModel:
        ss << " -wiresflavor transition";
        break;
    case WireModel::oldWireModel:
        ss << " -wiresflavor old";
        break;
    }
    switch (getInductanceModel()) {
    case InductanceModel::boutayeb:
        break;
    case InductanceModel::ledfelt:
        ss << " -inductance ledfelt";
        break;
    case InductanceModel::berenger:
        ss << " -inductance berenger";
        break;
    }
    ss << toStrIfTrue(" -mtln", isMTLN());
    ss << toStrIfTrue(" -taparrabos", isTaparrabos());
    ss << toStrIfTrue(" -groundwires", isGroundWires());
    ss << toStrIfTrue(" -makeholes", isMakeHoles());
    if (getWiresAttenuationFactor() != defaultOptions.getWiresAttenuationFactor()) {
        ss << " -attw " << getWiresAttenuationFactor();
    }
    // --- PMLs.
    if (!isUseDefaultPml()) {
        ss << " -pmlalpha " << getPmlAlpha().first << " " << getPmlAlpha().second;
        ss << " -pmlkappa " << getPmlKappa();
        ss << " -pmlcorr " << getPmlCorrection().first << " " << getPmlCorrection().second;
        switch (getPMLBacking()) {
        case PMLBacking::mur1:
            ss << " -mur1";
            break;
        case PMLBacking::mur2:
            ss << " -mur2";
            break;
        case PMLBacking::none:
            break;
        }
    }
    // --- Other options.
    switch (getNoNF2FF()) {
    case NoNF2FF::none:
        break;
    case NoNF2FF::back:
        ss << " -noNF2FF back";
        break;
    case NoNF2FF::front:
        ss << " -noNF2FF front";
        break;
    case NoNF2FF::left:
        ss << " -noNF2FF left";
        break;
    case NoNF2FF::right:
        ss << " -noNF2FF right";
        break;
    case NoNF2FF::down:
        ss << " -noNF2FF down";
        break;
    case NoNF2FF::up:
        ss << " -noNF2FF up";
        break;
    }
    ss << toStrIfTrue(" -NF2FFdecim", isNF2FFDecimation());
    ss << toStrIfTrue(" -map", isMap());
    ss << toStrIfTrue(" -mapvtk", isMapVtk());
    ss << " " << getAdditionalArguments();
    return ss.str();
}

const string& OptionsSolver::getAdditionalArguments() const {
    return additionalArguments_;
}

void OptionsSolver::setAdditionalArguments(const string& additionalArguments) {
    additionalArguments_ = additionalArguments;
}

bool OptionsSolver::isConformalSkin() const {
    return conformalSkin_;
}

void OptionsSolver::setConformalSkin(bool conformalSkin) {
    conformalSkin_ = conformalSkin;
}

Real OptionsSolver::getFlush() const {
    return flush_;
}

void OptionsSolver::setFlush(Real flush) {
    flush_ = flush;
}

bool OptionsSolver::isForceRestarting() const {
    return forceRestarting_;
}

void OptionsSolver::setForceRestarting(bool forceRestarting) {
    forceRestarting_ = forceRestarting;
}

bool OptionsSolver::isMap() const {
    return map_;
}

void OptionsSolver::setMap(bool map) {
    map_ = map;
}

bool OptionsSolver::isMapVtk() const {
    return mapVTK_;
}

void OptionsSolver::setMapVtk(bool mapVtk) {
    mapVTK_ = mapVtk;
}

Real OptionsSolver::getMinDistanceWires() const {
    return minDistanceWires_;
}

void OptionsSolver::setMinDistanceWires(Real minDistanceWires) {
    minDistanceWires_ = minDistanceWires;
}

bool OptionsSolver::isNewDispersiveFormulation() const {
    return newDispersiveFormulation_;
}

void OptionsSolver::setNewDispersiveFormulation(bool newDispersiveFormulation) {
    newDispersiveFormulation_ = newDispersiveFormulation;
}

bool OptionsSolver::isNF2FFDecimation() const {
    return nF2FFDecimation_;
}

void OptionsSolver::setNF2FFDecimation(bool f2FfDecimation) {
    nF2FFDecimation_ = f2FfDecimation;
}

bool OptionsSolver::isNoCompoMur() const {
    return noCompoMur_;
}

void OptionsSolver::setNoCompoMur(bool noCompoMur) {
    noCompoMur_ = noCompoMur;
}

OptionsSolver::NoNF2FF OptionsSolver::getNoNF2FF() const {
    return noNF2FF_;
}

void OptionsSolver::setNoNF2FF(NoNF2FF noNf2Ff) {
    noNF2FF_ = noNf2Ff;
}

bool OptionsSolver::isResumeSimulation() const {
    return resumeSimulation_;
}

void OptionsSolver::setResumeSimulation(bool resumeSimulation) {
    resumeSimulation_ = resumeSimulation;
}

bool OptionsSolver::isSkinDepth() const {
    return skinDepth_;
}

void OptionsSolver::setSkinDepth(bool skinDepth) {
    skinDepth_ = skinDepth;
}

bool OptionsSolver::isUseDefaultPml() const {
    return useDefaultPML_;
}

void OptionsSolver::setUseDefaultPml(bool useDefaultPml) {
    useDefaultPML_ = useDefaultPml;
}

OptionsSolver::PMLBacking OptionsSolver::getPMLBacking() const {
    return pmlBacking_;
}

void OptionsSolver::setPMLBacking(PMLBacking pmlBacking) {
    pmlBacking_ = pmlBacking;
}

string OptionsSolver::toStrIfTrue(const string str, const bool param) {
    if (param) {
        return str;
    } else {
        return string();
    }
}
