#include "EMSource.h"

EMSource<void>::EMSource() {
    magnitude_ = NULL;
}

EMSource<void>::EMSource(const Magnitude* magnitude) {
    magnitude_ = magnitude;
}

EMSource<void>::EMSource(const EMSource& rhs) {
    magnitude_ = rhs.magnitude_->clone()->castTo<Magnitude>();
}

EMSource<void>::~EMSource() {

}

string EMSource<void>::getMagnitudeFilename() const {
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

void EMSource<void>::convertToNumerical(const string file,
                                        const double step,
                                        const double finalTime) {
    if(magnitude_->is<MagnitudeNumerical>()) {
        return;
    }
    const Magnitude* orig = magnitude_;
    magnitude_ = new MagnitudeNumerical(file, magnitude_, step, finalTime);
    delete orig;
}

void EMSource<void>::printInfo() const {
    magnitude_->printInfo();
}

const Magnitude* EMSource<void>::getMagnitude() const {
    return magnitude_;
}
