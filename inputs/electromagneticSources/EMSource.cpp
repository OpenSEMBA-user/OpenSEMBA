#ifndef ELECTROMAGNETICSOURCE_H_
#	include "EMSource.h"
#endif

EMSource::EMSource() {
    magnitude_ = NULL;
}

EMSource::EMSource(const ElementsGroup<>& elem, const Magnitude* magnitude)
: ElementsGroup<>(elem) {
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
    cout<< " - Assigned on " << size() << " elements." << endl;
    magnitude_->printInfo();
}
