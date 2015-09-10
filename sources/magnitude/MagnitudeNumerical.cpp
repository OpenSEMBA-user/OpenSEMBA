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
    ofstream out;
    out.open(file.c_str());
    Real time = 0.0;
    for (UInt i = 0; i < nSteps; i++) {
        // Determines if neigh values are aligned with current.
        vector<pair<Real,Real>> preAndPost;
        const Real tPre = time - timeStep;
        const Real tPost = time + timeStep;
        preAndPost.push_back(pair<Real,Real>(tPre, mag.evaluate(tPre)));
        preAndPost.push_back(pair<Real,Real>(tPost, mag.evaluate(tPost)));
        const Real interpolated = LinearInterpolation(preAndPost)(time);
        const Real current = mag.evaluate(time);
        bool isAligned = MathUtils::equal(current, interpolated);
        //
        if (!isAligned || i == nSteps-1) {
            out << time << " " << current << endl;
        }
        //
        time += timeStep;
    }
    out.close();

    Magnitude::operator=(Magnitude(new LinearInterpolation(file)));
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
