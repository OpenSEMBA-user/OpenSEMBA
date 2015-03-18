#ifndef ELECTROMAGNETICSOURCE_H_
#	include "EMSource.h"
#endif

EMSource::EMSource() {
    magnitude_ = NULL;
}

EMSource::EMSource(const Magnitude* magnitude) {
    magnitude_ = magnitude;
}


EMSource::~EMSource() {

}

const Magnitude*
EMSource::getMagnitude() const {
    return magnitude_;
}

Condition::Type EMSource::getConditionType() const {
    return Condition::emSource;
}

void
EMSource::printInfo() const {
    cout << " --- EMSource info --- " << endl;
    magnitude_->printInfo();
}

string EMSource::getMagnitudeFilename() const {
    const MagnitudeNumerical* mag =
            dynamic_cast<const MagnitudeNumerical*>(magnitude_);
    if (mag != NULL) {
        return mag->getFilename();
    }
    cerr << endl << "ERROR @ EMSource: Magnitude is not numerical." << endl;
    printInfo();
    return string();
}

void EMSource::convertToNumerical(
        const string file,
        const double step,
        const double finalTime) {
    const MagnitudeNumerical* mag =
            dynamic_cast<const MagnitudeNumerical*>(magnitude_);
    if(mag != NULL) {
        return;
    }
    const Magnitude* orig = magnitude_;
    magnitude_ = new MagnitudeNumerical(file, magnitude_, step, finalTime);
    delete orig;
}
