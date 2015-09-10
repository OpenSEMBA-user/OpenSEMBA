/*
 * MagnitudeNumerical.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: luis
 */

#include "../../sources/magnitude/MagnitudeNumerical.h"

MagnitudeNumerical::MagnitudeNumerical() {

}

MagnitudeNumerical::MagnitudeNumerical(const ProjectFile& file)
:   Magnitude(new LinearInterpolation(file)),
    ProjectFile(file) {
}

MagnitudeNumerical::MagnitudeNumerical(const ProjectFile& file,
                                       const Magnitude& mag,
                                       const Real timeStep,
                                       const Real finalTime)
:   ProjectFile(file) {

    if(mag.is<MagnitudeNumerical>()) {
        operator=(*mag.castTo<MagnitudeNumerical>());
        return;
    }
    UInt nSteps;
    if (timeStep != 0.0) {
        nSteps = abs(finalTime / timeStep);
    } else {
        nSteps = defaultNumberOfSteps;
        cerr << "WARNING @ MagnitudeNumerical: "
             << "Attempting to build a numerical magnitude with a 0.0 step."
             << "Using default number of steps instead: " << nSteps << endl;
        mag.printInfo();
    }
    ofstream oStream;
    oStream.open(file.c_str());
    Real time = 0.0;
    for (UInt i = 0; i < nSteps; i++) {
        oStream << time << " " << mag.evaluate(time) << endl;
        time += timeStep;
    }
    oStream.close();

    Magnitude(new LinearInterpolation(file));
}

MagnitudeNumerical::~MagnitudeNumerical() {

}

MagnitudeNumerical& MagnitudeNumerical::operator=(
        const MagnitudeNumerical& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Magnitude::operator=(rhs);
    ProjectFile::operator=(rhs);

    return *this;
}

bool MagnitudeNumerical::operator==(const Magnitude& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    bool areEqual = true;
    areEqual &= Magnitude::operator==(rhs);
    const MagnitudeNumerical* rhsPtr = rhs.castTo<MagnitudeNumerical>();
    areEqual &= (ProjectFile::compare(*rhsPtr) == 0);
    return areEqual;
}

Real MagnitudeNumerical::evaluate(const Real time) const {
    throw ErrorNotImplemented("MagnitudeNumerical::evaluate");
    return 0.0;
}

void MagnitudeNumerical::printInfo() const {
    cout << " --- Magnitude Numerical Info --- " << endl;
    Magnitude::printInfo();
    ProjectFile::printInfo();
}
