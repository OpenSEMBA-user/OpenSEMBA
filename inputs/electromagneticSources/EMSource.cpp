#ifndef ELECTROMAGNETICSOURCE_H_
#	include "EMSource.h"
#endif

EMSource::EMSource() {
    magnitude_ = NULL;
}

EMSource::EMSource(const vector<uint>& elem, const Magnitude* magnitude) {
    elem_ = elem;
    magnitude_ = magnitude;
    assert(elem_.size() != 0);
}


EMSource::~EMSource() {

}

vector<unsigned int>
EMSource::getElem() const {
    assert(elem_.size() != 0);
    return elem_;
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
    cout<< " - Assigned on " << elem_.size() << ":" << endl;
    for (uint i = 0; i < elem_.size(); i++) {
        cout<< elem_[i] << " ";
    }
    cout << endl;
    magnitude_->printInfo();
}
