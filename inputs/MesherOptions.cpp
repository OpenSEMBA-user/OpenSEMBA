/*
 * OpenFOAMParameters.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "MesherOptions.h"

MesherOptions::MesherOptions() {
    locationInMeshSet_ = false;
    mesher_ = ugrMesher;
    mode_ = structured;
    bruteForceVolumes_ = false;
    scaleFactor_ = false;
    effectiveParameter_ = false;
    scalingFactor_ = 1.0;
    for (unsigned int i = 0; i < 3; i++) {
        boundTermination_[i].first = pml;
        boundTermination_[i].second = pml;
    }
}

MesherOptions::MesherOptions(
        Mesher mesher,
        bool locationInMeshSet,
        CVecD3 locationInMesh,
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
MesherOptions::printInfo() const {
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

string
MesherOptions::toStr(const BoundType val) const {
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
MesherOptions::applyGeometricScalingFactor(const double& factor) {
    boundaryPadding_.first *= factor;
    boundaryPadding_.second *= factor;
    boundaryMeshSize_.first *= factor;
    boundaryMeshSize_.second *= factor;
}

const CVecD3& MesherOptions::getLocationInMesh() const {
    return locationInMesh_;
}

bool MesherOptions::isLocationInMeshSet() const {
    return locationInMeshSet_;
}

bool MesherOptions::isBruteForceVolumes() const {
    return bruteForceVolumes_;
}

bool MesherOptions::hasEffParams() const {
    return effectiveParameter_;
}
string MesherOptions::getEffThick() const {
    return th_;
}
string MesherOptions::getEffSigma() const {
    return sigma_;
}

bool MesherOptions::isStructured() const {
    return (mode_ == structured);
}

bool MesherOptions::isRelaxed() const {
    return (mode_ == relaxed);
}

bool MesherOptions::isSlanted() const {
    return (mode_ == slanted);
}

string MesherOptions::getEdgeFraction() const {
    return edgeFraction_;
}

bool MesherOptions::hasScaleFactor() const {
    return scaleFactor_;
}

string MesherOptions::getScaleFactor() const {
    return scaleFactorValue_;
}

string MesherOptions::getSWFForce() const {
    return swfForze_;
}

void MesherOptions::setBruteForceVolumes(bool bruteForceVolumes) {
    bruteForceVolumes_ = bruteForceVolumes;
}

const string& MesherOptions::getMeshOutputName() const {
    return confOutput_;
}

void MesherOptions::setConfOutput(const string& confOutput) {
    confOutput_ = confOutput;
}

void MesherOptions::setEdgeFraction(const string& edgeFraction) {
    edgeFraction_ = edgeFraction;
}

bool MesherOptions::isEffectiveParameter() const {
    return effectiveParameter_;
}

void MesherOptions::setEffectiveParameter(bool effectiveParameter) {
    effectiveParameter_ = effectiveParameter;
}

void MesherOptions::setLocationInMesh(const CVecD3& locationInMesh) {
    locationInMesh_ = locationInMesh;
    locationInMeshSet_ = true;
}

MesherOptions::Mesher MesherOptions::getMesher() const {
    return mesher_;
}

void MesherOptions::setMesher(Mesher mesher) {
    mesher_ = mesher;
}

MesherOptions::Mode MesherOptions::getMode() const {
    return mode_;
}

void MesherOptions::setMode(Mode mode) {
    mode_ = mode;
}

void MesherOptions::setScaleFactor(bool scaleFactor) {
    scaleFactor_ = scaleFactor;
}

void MesherOptions::setScaleFactorValue(const string& scaleFactorValue) {
    scaleFactorValue_ = scaleFactorValue;
}

void MesherOptions::setSigma(const string& sigma) {
    sigma_ = sigma;
}

void MesherOptions::setSwfForze(const string& swfForze) {
    swfForze_ = swfForze;
}

void MesherOptions::setTh(const string& th) {
    th_ = th;
}

const pair<CVecD3, CVecD3>& MesherOptions::getBoundaryMeshSize() const {
    return boundaryMeshSize_;
}

void MesherOptions::setBoundaryMeshSize(
        const pair<CVecD3, CVecD3>& boundaryMeshSize) {
    boundaryMeshSize_ = boundaryMeshSize;
}

const pair<CVecD3, CVecD3>& MesherOptions::getBoundaryPadding() const {
    return boundaryPadding_;
}

void MesherOptions::setBoundaryPadding(
        const pair<CVecD3, CVecD3>& boundaryPadding) {
    boundaryPadding_ = boundaryPadding;
}

double MesherOptions::getScalingFactor() const {
    return scalingFactor_;
}

void MesherOptions::setScalingFactor(double scalingFactor) {
    scalingFactor_ = scalingFactor;
}
pair<MesherOptions::BoundType, MesherOptions::BoundType>
MesherOptions::getBoundTermination(
        const uint i) const {
    assert(i < 3);
    return boundTermination_[i];
}

void MesherOptions::setBoundTermination(
        const uint i,
        uint j,
        BoundType bound) {
    assert(i < 3);
    assert(j < 2);
    if (j == 0) {
        boundTermination_[i].first = bound;
    } else {
        boundTermination_[i].second = bound;
    }
}
