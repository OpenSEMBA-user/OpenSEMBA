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
    mode_ = conformal;
    bruteForceVolumes_ = false;
    scaleFactor_ = false;
    vtkExport_ = false;
    gridStep_ = CVecR3(0.0);
    forbiddenLength_ = (Real) (1.0 / 3.0);
    boundTermination_.resize(3);
    for (UInt i = 0; i < 3; i++) {
        boundTermination_[i].first = pml;
        boundTermination_[i].second = pml;
    }
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
    cout<< "Lower X Bound: " << toStr(boundTermination_[0].first) << endl;
    cout<< "Upper X Bound: " << toStr(boundTermination_[0].second) << endl;
    cout<< "Lower Y Bound: " << toStr(boundTermination_[1].first) << endl;
    cout<< "Upper Y Bound: " << toStr(boundTermination_[1].second) << endl;
    cout<< "Lower Z Bound: " << toStr(boundTermination_[2].first) << endl;
    cout<< "Upper Z Bound: " << toStr(boundTermination_[2].second) << endl;
    cout << " --- End of Meshing parameters info ---" << endl;
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

const CVecR3& OptionsMesher::getLocationInMesh() const {
    return locationInMesh_;
}

bool OptionsMesher::isLocationInMeshSet() const {
    return locationInMeshSet_;
}

bool OptionsMesher::isBruteForceVolumes() const {
    return bruteForceVolumes_;
}

bool OptionsMesher::isStructured() const {
    return (mode_ == structured);
}

bool OptionsMesher::isRelaxed() const {
    if (mode_ == slanted || mode_ == relaxed) {
        return true;
    }
    return false;
}

bool OptionsMesher::hasScaleFactor() const {
    return scaleFactor_;
}

string OptionsMesher::getScaleFactor() const {
    return scaleFactorValue_;
}

OptionsMesher::Mesher OptionsMesher::getMesher() const {
    return mesher_;
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
        mode_  = structured;
    }
    if (args.has("slanted")) {
        mode_ = relaxed;
        forbiddenLength_ = - 1.0;
        forbiddenLength_ = atof(args.get("slanted").c_str());
    }
    if (args.has("relaxed")) {
        mode_ = relaxed;
        forbiddenLength_ = atof(args.get("relaxed").c_str());
    }
    if (args.has("vtkexport")) {
        vtkExport_ = true;
    }
    if (args.has("gridstep")) {
        CVecR3 gridStep;
        try {
            for (UInt i = 0; i < 3; i++) {
                gridStep(i) = atof(args.get("gridstep", i).c_str());
            }
        }
        catch (Arguments::ErrorArgumentNotExists &e) {
            cerr << endl << "ERROR @ Arguments: "
                    << "gridStep must be followed by 3 numbers." << endl;
        }
        gridStep_ = gridStep;
    }
}

void OptionsMesher::setBruteForceVolumes(bool bruteForceVolumes) {
    bruteForceVolumes_ = bruteForceVolumes;
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

void OptionsMesher::setForbiddenLength(const Real& edgeFraction) {
}

Real OptionsMesher::getForbiddenLength() const {
    if (mode_ == Mode::slanted) {
        return - 1.0;
    } else {
        return forbiddenLength_;
    }
}

const string& OptionsMesher::getOutputName() const {
    return outputName_;
}

void OptionsMesher::setOutputName(const string& outputName) {
    outputName_ = outputName;
}

const CVecR3& OptionsMesher::getGridStep() const {
    return gridStep_;
}

void OptionsMesher::setGridStep(const CVecR3& gridStep) {
    gridStep_ = gridStep;
}

bool OptionsMesher::isVtkExport() const {
    return vtkExport_;
}

void OptionsMesher::setVtkExport(bool vtkExport) {
    vtkExport_ = vtkExport;
}

bool OptionsMesher::isGridStepSet() const {
    return (gridStep_ != CVecR3(0.0));
}
