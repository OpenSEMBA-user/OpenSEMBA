/*
 * OpenFOAMParameters.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "MesherParameters.h"

MesherParameters::MesherParameters() {
    locationInMeshSet_ = false;
    mesher_ = ugrMesher;
    mode_ = structured;
    bruteForceVolumes_ = false;
    scaleFactor_ = false;
    effectiveParameter_ = false;
}

MesherParameters::MesherParameters(
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
}

void
MesherParameters::printInfo() const {
    cout<< " --- Meshing parameters info --- " << endl;
    if (mesher_ == openfoam) {
        cout << "- Using openfoam mesher." << endl;
        if (locationInMeshSet_) {
            cout << "- Location in mesh:   " << locationInMesh_ << endl;
        }
    }
    if (isStructured()) {
        cout << "- Generating structured mesh." << endl;
    }
    cout << " --- End of Meshing parameters info ---" << endl;
}

const CVecR3& MesherParameters::getLocationInMesh() const {
    return locationInMesh_;
}

bool MesherParameters::isLocationInMeshSet() const {
    return locationInMeshSet_;
}

bool MesherParameters::isBruteForceVolumes() const {
    return bruteForceVolumes_;
}

bool MesherParameters::hasEffParams() const {
    return effectiveParameter_;
}
string MesherParameters::getEffThick() const {
    return th_;
}
string MesherParameters::getEffSigma() const {
    return sigma_;
}

bool MesherParameters::isStructured() const {
    return (mode_ == structured);
}

bool MesherParameters::isRelaxed() const {
    return (mode_ == relaxed);
}

bool MesherParameters::isSlanted() const {
    return (mode_ == slanted);
}

string MesherParameters::getEdgeFraction() const {
    return edgeFraction_;
}

bool MesherParameters::hasScaleFactor() const {
    return scaleFactor_;
}

string MesherParameters::getScaleFactor() const {
    return scaleFactorValue_;
}

string MesherParameters::getSWFForce() const {
    return swfForze_;
}

void MesherParameters::setBruteForceVolumes(bool bruteForceVolumes) {
    bruteForceVolumes_ = bruteForceVolumes;
}

const string& MesherParameters::getMeshOutputName() const {
    return confOutput_;
}

void MesherParameters::setConfOutput(const string& confOutput) {
    confOutput_ = confOutput;
}

void MesherParameters::setEdgeFraction(const string& edgeFraction) {
    edgeFraction_ = edgeFraction;
}

bool MesherParameters::isEffectiveParameter() const {
    return effectiveParameter_;
}

void MesherParameters::setEffectiveParameter(bool effectiveParameter) {
    effectiveParameter_ = effectiveParameter;
}

void MesherParameters::setLocationInMesh(const CVecR3& locationInMesh) {
    locationInMesh_ = locationInMesh;
    locationInMeshSet_ = true;
}

MesherParameters::Mesher MesherParameters::getMesher() const {
    return mesher_;
}

void MesherParameters::setMesher(Mesher mesher) {
    mesher_ = mesher;
}

MesherParameters::Mode MesherParameters::getMode() const {
    return mode_;
}

void MesherParameters::setMode(Mode mode) {
    mode_ = mode;
}

void MesherParameters::setScaleFactor(bool scaleFactor) {
    scaleFactor_ = scaleFactor;
}

void MesherParameters::setScaleFactorValue(const string& scaleFactorValue) {
    scaleFactorValue_ = scaleFactorValue;
}

void MesherParameters::setSigma(const string& sigma) {
    sigma_ = sigma;
}

void MesherParameters::setSwfForze(const string& swfForze) {
    swfForze_ = swfForze;
}

void MesherParameters::setTh(const string& th) {
    th_ = th;
}
