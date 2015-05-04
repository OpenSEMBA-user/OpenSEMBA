/*
 * OpenFOAMParameters.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "OptionsMesher.h"

OptionsMesher::OptionsMesher() {
    locationInMeshSet_ = false;
    mesher_ = ugrMesher;
    mode_ = structured;
    bruteForceVolumes_ = false;
    scaleFactor_ = false;
    effectiveParameter_ = false;
    scalingFactor_ = 1.0;
    boundTermination_.resize(3);
    for (UInt i = 0; i < 3; i++) {
        boundTermination_[i].first = pml;
        boundTermination_[i].second = pml;
    }
}

OptionsMesher::OptionsMesher(
        Mesher mesher,
        bool locationInMeshSet,
        CVecR3 locationInMesh,
        bool bruteForceVolumes,
        Mode mode,
        bool effectiveParameter,
        string th,
        string sigma,
        string edgeFraction,
        bool scaleFactor,
        string scaleFactorValue,
        string swfForze,
        string confOutput) {
    mesher_ = mesher;
    locationInMeshSet_ = locationInMeshSet;
    locationInMesh_ = locationInMesh;
    bruteForceVolumes_ = bruteForceVolumes;
    mode_ = mode;
    effectiveParameter_ = effectiveParameter;
    th_ = th;
    sigma_ = sigma;
    edgeFraction_ = edgeFraction;
    scaleFactor_ = scaleFactor;
    scaleFactorValue_ = scaleFactorValue;
    swfForze_ = swfForze;
    confOutput_ = confOutput;
    scalingFactor_ = 0.0;
}

void
OptionsMesher::printInfo() const {
    cout<< " --- Meshing parameters info --- " << endl;
    if (mesher_ == openfoam) {
        cout << " Using openfoam mesher." << endl;
        if (locationInMeshSet_) {
            cout << " Location in mesh:   " << locationInMesh_ << endl;
        }
    }
    if (isStructured()) {
        cout << " Generating structured mesh." << endl;
    }
    cout<< "Geom. scaling factor: " << scalingFactor_ << endl;
    cout<< "Lower X Bound: " << toStr(boundTermination_[0].first) << endl;
    cout<< "Upper X Bound: " << toStr(boundTermination_[0].second) << endl;
    cout<< "Lower Y Bound: " << toStr(boundTermination_[1].first) << endl;
    cout<< "Upper Y Bound: " << toStr(boundTermination_[1].second) << endl;
    cout<< "Lower Z Bound: " << toStr(boundTermination_[2].first) << endl;
    cout<< "Upper Z Bound: " << toStr(boundTermination_[2].second) << endl;
    cout<< "Lower boundary padding: " << boundaryPadding_.first << endl;
    cout<< "Upper boundary padding: " << boundaryPadding_.second << endl;
    cout<< "Lower boundary mesh size: " << boundaryMeshSize_.first << endl;
    cout<< "Upper boundary mesh size: " << boundaryMeshSize_.second << endl;
    cout << " --- End of Meshing parameters info ---" << endl;
}

void OptionsMesher::setTh(const string& th) {
    th_ = th;
}

void OptionsMesher::printHelp() const {
}

BoundTerminations OptionsMesher::getBoundTerminations() const {
    return boundTermination_;
}

string
OptionsMesher::toStr(const BoundType val) {
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
    default:
        return "Undefined";
    }
}

void
OptionsMesher::applyGeometricScalingFactor(const Real& factor) {
    boundaryPadding_.first *= factor;
    boundaryPadding_.second *= factor;
    boundaryMeshSize_.first *= factor;
    boundaryMeshSize_.second *= factor;
}

const CVecR3& OptionsMesher::getLocationInMesh() const {
    return locationInMesh_;
}

bool OptionsMesher::isLocationInMeshSet() const {
    return locationInMeshSet_;
}

bool OptionsMesher::isBruteForceVolumes() const {
    return bruteForceVolumes_;
}

bool OptionsMesher::hasEffParams() const {
    return effectiveParameter_;
}
string OptionsMesher::getEffThick() const {
    return th_;
}
string OptionsMesher::getEffSigma() const {
    return sigma_;
}

bool OptionsMesher::isStructured() const {
    return (mode_ == structured);
}

bool OptionsMesher::isRelaxed() const {
    return (mode_ == relaxed);
}

bool OptionsMesher::isSlanted() const {
    return (mode_ == slanted);
}

string OptionsMesher::getEdgeFraction() const {
    return edgeFraction_;
}

bool OptionsMesher::hasScaleFactor() const {
    return scaleFactor_;
}

string OptionsMesher::getScaleFactor() const {
    return scaleFactorValue_;
}

string OptionsMesher::getSWFForce() const {
    return swfForze_;
}

const string& OptionsMesher::getMeshOutputName() const {
    return confOutput_;
}

bool OptionsMesher::isEffectiveParameter() const {
    return effectiveParameter_;
}

OptionsMesher::Mesher OptionsMesher::getMesher() const {
    return mesher_;
}

const pair<CVecR3, CVecR3>& OptionsMesher::getBoundaryMeshSize() const {
    return boundaryMeshSize_;
}

void OptionsMesher::setBoundaryMeshSize(
        const pair<CVecR3, CVecR3>& boundaryMeshSize) {
    boundaryMeshSize_ = boundaryMeshSize;
}

const pair<CVecR3, CVecR3>& OptionsMesher::getBoundaryPadding() const {
    return boundaryPadding_;
}

void OptionsMesher::setBoundaryPadding(
        const pair<CVecR3, CVecR3>& boundaryPadding) {
    boundaryPadding_ = boundaryPadding;
}

Real OptionsMesher::getScalingFactor() const {
    return scalingFactor_;
}

void OptionsMesher::setScalingFactor(Real scalingFactor) {
    scalingFactor_ = scalingFactor;
}
OptionsMesher::BoundType
OptionsMesher::getBoundTermination(
        const UInt i, const UInt p) const {
    assert(i < 3);
    assert(p < 2);
    if (p == 0) {
        return boundTermination_[i].first;
    } else {
        return boundTermination_[i].second;
    }
}

void OptionsMesher::setBoundTermination(
        const UInt i,
        UInt j,
        BoundType bound) {
    assert(i < 3);
    assert(j < 2);
    if (j == 0) {
        boundTermination_[i].first = bound;
    } else {
        boundTermination_[i].second = bound;
    }
}

void OptionsMesher::set(const Arguments& args) {
    if (args.has("h") || args.has("help")) {
        printHelp();
        exit(EXIT_SUCCESS);
    }
    if (args.has("openfoam")) {
        mesher_ = openfoam;
    }
    if (args.has("bruteForceVolumes")) {
        bruteForceVolumes_ = true;
    }
    if (args.has("structured")) {
        #ifndef compileOnlyWithStructured
        #ifdef compileWithstaircase
            mode_  = structured;
        #endif
        #endif
    }
    if (args.has("eff")) {
        #ifdef effectiveParameterMethods
            effectiveParameter_ = true;
            th_    = args.get("eff",0);
            sigma_ = args.get("eff",1);
        #endif
    }
    if (args.has("slanted")) {
        #ifndef compileOnlyWithStructured
            mode_ = slanted;
            edgeFraction_ = args.get("slanted");
        #endif
    }
    if (args.has("relaxed")) {
        #ifndef compileOnlyWithStructured
            mode_ = relaxed;
            edgeFraction_ = args.get("relaxed");
        #endif
    }
    if (args.has("scalefactor")) {
        #ifdef compileWithTesting
        scaleFactor_ = true;
        scaleFactorValue_ = args.get("scalefactor");
        #endif
    }
    if (args.has("swff")) {
        swfForze_ = args.get("swff");
    }
}

void OptionsMesher::setBruteForceVolumes(bool bruteForceVolumes) {
    bruteForceVolumes_ = bruteForceVolumes;
}

void OptionsMesher::setConfOutput(const string& confOutput) {
    confOutput_ = confOutput;
}

void OptionsMesher::setEdgeFraction(const string& edgeFraction) {
    edgeFraction_ = edgeFraction;
}

void OptionsMesher::setLocationInMesh(const CVecR3& locationInMesh) {
    locationInMeshSet_ = true;
    locationInMesh_ = locationInMesh;
}

void OptionsMesher::setLocationInMeshSet(bool locationInMeshSet) {
    locationInMeshSet_ = locationInMeshSet;
}

void OptionsMesher::setMesher(Mesher mesher) {
    mesher_ = mesher;
}

void OptionsMesher::setMode(Mode mode) {
    mode_ = mode;
}

void OptionsMesher::setScaleFactor(bool scaleFactor) {
    scaleFactor_ = scaleFactor;
}

void OptionsMesher::setScaleFactorValue(const string& scaleFactorValue) {
    scaleFactorValue_ = scaleFactorValue;
}

void OptionsMesher::setSigma(const string& sigma) {
    sigma_ = sigma;
}

void OptionsMesher::setSwfForze(const string& swfForze) {
    swfForze_ = swfForze;
}

void OptionsMesher::setEffectiveParameter(bool effectiveParameter) {
    effectiveParameter_ = effectiveParameter;
}
