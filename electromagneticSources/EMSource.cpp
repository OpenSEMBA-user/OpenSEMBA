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
    return string();
}

void EMSourceBase::convertToNumerical(const string file,
                                      const Real step,
                                      const Real finalTime) {
    if(magnitude_->is<MagnitudeNumerical>()) {
        return;
    }
    const Magnitude* orig = magnitude_;
    magnitude_ = new MagnitudeNumerical(file, magnitude_, step, finalTime);
    delete orig;
}

MagnitudeNumerical* EMSourceBase::exportToFile(const string file,
                                               const Real step,
                                               const Real finalTime) const {

    if(magnitude_->is<MagnitudeNumerical>()) {
        return magnitude_->cloneTo<MagnitudeNumerical>();
    }
    return new MagnitudeNumerical(file, magnitude_, step, finalTime);
}

void EMSourceBase::printInfo() const {
    magnitude_->printInfo();
}

const Magnitude* EMSourceBase::getMagnitude() const {
    return magnitude_;
}
