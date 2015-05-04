#include "EMSource.h"

EMSourceBase::EMSourceBase() {
    magnitude_ = NULL;
}

EMSourceBase::EMSourceBase(const Magnitude* magnitude) {
    magnitude_ = magnitude;
}

EMSourceBase::EMSourceBase(const EMSourceBase& rhs) {
    if (magnitude_ != NULL) {
        magnitude_ = rhs.magnitude_->cloneTo<Magnitude>();
    } else {
        magnitude_ = rhs.magnitude_;
    }
}

EMSourceBase::~EMSourceBase() {

}

bool EMSourceBase::hasSameProperties(const EMSourceBase& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    return (*magnitude_ == *rhs.magnitude_);
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
    cout << " --- EMSource Base Info ---" << endl;
    if (magnitude_ != NULL) {
        magnitude_->printInfo();
    } else {
        cout << "No magnitude defined." << endl;
    }
}

const Magnitude* EMSourceBase::getMagnitude() const {
    return magnitude_;
}
