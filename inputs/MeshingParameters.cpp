/*
 * OpenFOAMParameters.cpp
 *
 *  Created on: May 27, 2014
 *      Author: luis
 */

#include "MeshingParameters.h"

MeshingParameters::MeshingParameters() {
    locationInMeshSet_ = false;
    mesher_ = ugrMesher;
    mode_ = structured;
    bruteForceVolumes_ = false;
    scaleFactor_ = false;
    effectiveParameter_ = false;
}

MeshingParameters::MeshingParameters(
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
        string  swfForze) {
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
}

void
MeshingParameters::printInfo() const {
    cout<< " --- Meshing parameters info --- " << endl;
    if (useOpenFoam()) {
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

const CVecD3& MeshingParameters::getLocationInMesh() const {
    return locationInMesh_;
}

bool MeshingParameters::isLocationInMeshSet() const {
    return locationInMeshSet_;
}


bool MeshingParameters::useOpenFoam() const {
    return (mesher_ == openfoam);
}

bool MeshingParameters::isBruteForceVolumes() const {
    return bruteForceVolumes_;
}

bool MeshingParameters::hasEffParams() const {
    return effectiveParameter_;
}
string MeshingParameters::getEffThick() const {
    return th_;
}
string MeshingParameters::getEffSigma() const {
    return sigma_;
}

bool MeshingParameters::isStructured() const {
    return (mode_ == structured);
}

bool MeshingParameters::isRelaxed() const {
    return (mode_ == relaxed);
}

bool MeshingParameters::isSlanted() const {
    return (mode_ == slanted);
}

string MeshingParameters::getEdgeFraction() const {
    return edgeFraction_;
}

bool MeshingParameters::hasScaleFactor() const {
    return scaleFactor_;
}

string MeshingParameters::getScaleFactor() const {
    return scaleFactorValue_;
}

string MeshingParameters::getSWFForce() const {
    return swfForze_;
}
