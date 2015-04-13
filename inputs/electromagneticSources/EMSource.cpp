#include "EMSource.h"

EMSourceBase::EMSourceBase() {
    magnitude_ = NULL;
}

EMSourceBase::EMSourceBase(const Magnitude* magnitude) {
    magnitude_ = magnitude;
}

EMSourceBase::EMSourceBase(const EMSourceBase& rhs) {
    magnitude_ = rhs.magnitude_->cloneTo<Magnitude>();
}

EMSourceBase::~EMSourceBase() {

}

string EMSourceBase::getMagnitudeFilename() const {
    const MagnitudeNumerical* mag =
            dynamic_cast<const MagnitudeNumerical*>(magnitude_);
    if (mag != NULL) {
        return mag->getFilename();
    }
    cerr << endl << "ERROR @ EMSource: Magnitude is not numerical." << endl;
    printInfo();
    assert(false);
    return string();
}

void EMSourceBase::convertToNumerical(const string file,
                                      const double step,
                                      const double finalTime) {
    if(magnitude_->is<MagnitudeNumerical>()) {
        return;
    }
    const Magnitude* orig = magnitude_;
    magnitude_ = new MagnitudeNumerical(file, magnitude_, step, finalTime);
    delete orig;
}

void EMSourceBase::printInfo() const {
    magnitude_->printInfo();
}

const Magnitude* EMSourceBase::getMagnitude() const {
    return magnitude_;
}
