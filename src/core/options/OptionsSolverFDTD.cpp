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
 * OptionsSolverFDTD.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: luis
 */

#include "OptionsSolverFDTD.h"

void OptionsSolverFDTD::initDefaults_() {
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
    pmlAlpha_ = pair<Real, Real>(0.0, 1.0);
    pmlKappa_ = 1.0;
    pmlCorrection_ = pair<Real, Real>(1.0, 1.0);
    pmlBacking_ = PMLBacking::none;
    map_ = false;
    mapVTK_ = false;
    nF2FFDecimation_ = false;
    noNF2FF_ = NoNF2FF::none;
}

OptionsSolverFDTD::OptionsSolverFDTD(const OptionsSolver& base) :
    OptionsSolver(base) {
    initDefaults_();
}

OptionsSolverFDTD::OptionsSolverFDTD() {
    initDefaults_();
}

OptionsSolverFDTD::~OptionsSolverFDTD() {
}

void OptionsSolverFDTD::set(const Arguments& arg) {
    OptionsSolverFDTD::set(arg);
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
        pair<Real,Real> factorOrder;
        factorOrder.first = atof(arg.get("pmlalpha",0).c_str());
        factorOrder.second = atof(arg.get("pmlalpha",1).c_str());
        setPMLAlpha(factorOrder);
    }
    if (arg.has("pmlkappa")) {
        setPMLKappa(atof(arg.get("pmlkappa").c_str()));
    }
    if (arg.has("pmlcorr")) {
        pair<Real,Real> factorDepth;
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
            throw Error("Unrecognized wire flavor: " + flavor);
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

string OptionsSolverFDTD::toArgsStr() const {
    OptionsSolverFDTD defaultOptions;
    stringstream ss;
    ss << OptionsSolver::toArgsStr();
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

OptionsSolverFDTD::CompositeModel OptionsSolverFDTD::getCompositeModel() const {
    return compositeModel_;
}

void OptionsSolverFDTD::setCompositeModel(CompositeModel compositeModel) {
    compositeModel_ = compositeModel;
}

Real OptionsSolverFDTD::getCompositesAttenuationFactor() const {
    return compositesAttenuationFactor_;
}

void OptionsSolverFDTD::setCompositesAttenuationFactor(
        Real compositesAttenuationFactor) {
    compositesAttenuationFactor_ = compositesAttenuationFactor;
}

bool OptionsSolverFDTD::isGroundWires() const {
    return groundWires_;
}

void OptionsSolverFDTD::setGroundWires(bool groundWires) {
    groundWires_ = groundWires;
}

bool OptionsSolverFDTD::isMakeHoles() const {
    return makeHoles_;
}

void OptionsSolverFDTD::setMakeHoles(bool makeHoles) {
    makeHoles_ = makeHoles;
}

bool OptionsSolverFDTD::isMTLN() const {
    return MTLN_;
}

void OptionsSolverFDTD::setMTLN(bool mtln) {
    MTLN_ = mtln;
}

const pair<Real, Real>& OptionsSolverFDTD::getPmlAlpha() const {
    return pmlAlpha_;
}

void OptionsSolverFDTD::setPMLAlpha(const pair<Real, Real>& pmlAlpha) {
    pmlAlpha_ = pmlAlpha;
}

const pair<Real, Real>& OptionsSolverFDTD::getPmlCorrection() const {
    return pmlCorrection_;
}

void OptionsSolverFDTD::setPMLCorrection(const pair<Real, Real>& pmlCorrection) {
    pmlCorrection_ = pmlCorrection;
}

Real OptionsSolverFDTD::getPmlKappa() const {
    return pmlKappa_;
}

void OptionsSolverFDTD::setPMLKappa(Real pmlKappa) {
    pmlKappa_ = pmlKappa;
}

OptionsSolverFDTD::InductanceModel OptionsSolverFDTD::getInductanceModel() const {
    return inductanceModel_;
}

void OptionsSolverFDTD::setInductanceModel(InductanceModel selfInductanceModel) {
    inductanceModel_ = selfInductanceModel;
}

bool OptionsSolverFDTD::isTaparrabos() const {
    return taparrabos_;
}

void OptionsSolverFDTD::setTaparrabos(bool taparrabos) {
    taparrabos_ = taparrabos;
}

OptionsSolverFDTD::WireModel OptionsSolverFDTD::getWireModel() const {
    return wireModel_;
}

void OptionsSolverFDTD::setWireModel(WireModel wireModel) {
    wireModel_ = wireModel;
}

Real OptionsSolverFDTD::getWiresAttenuationFactor() const {
    return wiresAttenuationFactor_;
}

void OptionsSolverFDTD::setWiresAttenuationFactor(Real wiresAttenuationFactor) {
    wiresAttenuationFactor_ = wiresAttenuationFactor;
}

bool OptionsSolverFDTD::isConformalSkin() const {
    return conformalSkin_;
}

void OptionsSolverFDTD::setConformalSkin(bool conformalSkin) {
    conformalSkin_ = conformalSkin;
}

bool OptionsSolverFDTD::isMapVtk() const {
    return mapVTK_;
}

void OptionsSolverFDTD::setMapVtk(bool mapVtk) {
    mapVTK_ = mapVtk;
}

Real OptionsSolverFDTD::getMinDistanceWires() const {
    return minDistanceWires_;
}

void OptionsSolverFDTD::setMinDistanceWires(Real minDistanceWires) {
    minDistanceWires_ = minDistanceWires;
}

bool OptionsSolverFDTD::isNewDispersiveFormulation() const {
    return newDispersiveFormulation_;
}

void OptionsSolverFDTD::setNewDispersiveFormulation(bool newDispersiveFormulation) {
    newDispersiveFormulation_ = newDispersiveFormulation;
}

bool OptionsSolverFDTD::isNF2FFDecimation() const {
    return nF2FFDecimation_;
}

void OptionsSolverFDTD::setNF2FFDecimation(bool f2FfDecimation) {
    nF2FFDecimation_ = f2FfDecimation;
}

bool OptionsSolverFDTD::isNoCompoMur() const {
    return noCompoMur_;
}

void OptionsSolverFDTD::setNoCompoMur(bool noCompoMur) {
    noCompoMur_ = noCompoMur;
}

OptionsSolverFDTD::NoNF2FF OptionsSolverFDTD::getNoNF2FF() const {
    return noNF2FF_;
}

void OptionsSolverFDTD::setNoNF2FF(NoNF2FF noNf2Ff) {
    noNF2FF_ = noNf2Ff;
}

bool OptionsSolverFDTD::isSkinDepth() const {
    return skinDepth_;
}

void OptionsSolverFDTD::setSkinDepth(bool skinDepth) {
    skinDepth_ = skinDepth;
}

bool OptionsSolverFDTD::isUseDefaultPml() const {
    return useDefaultPML_;
}

void OptionsSolverFDTD::setUseDefaultPml(bool useDefaultPml) {
    useDefaultPML_ = useDefaultPml;
}

OptionsSolverFDTD::PMLBacking OptionsSolverFDTD::getPMLBacking() const {
    return pmlBacking_;
}

void OptionsSolverFDTD::setPMLBacking(PMLBacking pmlBacking) {
    pmlBacking_ = pmlBacking;
}

bool OptionsSolverFDTD::isMap() const {
    return map_;
}

void OptionsSolverFDTD::setMap(bool map) {
    map_ = map;
}

string OptionsSolverFDTD::toStrIfTrue(const string str, const bool param) {
    if (param) {
        return str;
    } else {
        return string();
    }
}
