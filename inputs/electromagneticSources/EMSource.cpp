#ifndef ELECTROMAGNETICSOURCE_H_
#	include "EMSource.h"
#endif

EMSource::EMSource() {
    magnitude_ = NULL;
}

EMSource::~EMSource() {

}

vector<unsigned int>
EMSource::getElem() const {
	assert(elem_.size() != 0);
	return elem_;
}

bool
EMSource::isEMSource() const {
	return true;
}

bool
EMSource::isPlaneWave() const {
	return false;
}

EMSource::EMSource(const vector<uint>& elem, const Magnitude* magnitude) {
    elem_ = elem;
    magnitude_ = magnitude;
}

EMSource::EMSource(const Magnitude* magnitude) {
    magnitude_ = magnitude;
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

const Magnitude*
EMSource::getMagnitude() const {
    return magnitude_;
}
