#ifndef ELECTROMAGNETICSOURCE_H_
#	include "EMSource.h"
#endif

EMSource::EMSource() {
   bound_ = NULL;
    magnitude_ = NULL;
}

EMSource::EMSource(const vector<uint>& elem, const Magnitude* magnitude) {
   bound_ = NULL;
   elem_ = elem;
   magnitude_ = magnitude;
   assert(elem_.size() != 0);
}

EMSource::EMSource(const BoxD3& bound, const Magnitude* magnitude) {
   bound_ = new BoxD3(bound);
   magnitude_ = magnitude;
}

EMSource::~EMSource() {

}

vector<unsigned int>
EMSource::getElem() const {
	assert(elem_.size() != 0);
	return elem_;
}

const BoxD3*
EMSource::getBound() const {
   return bound_;
}

void
EMSource::printInfo() const {
    cout<< " - Assigned on " << elem_.size() << ":" << endl;
    for (uint i = 0; i < elem_.size(); i++) {
        cout<< elem_[i] << " ";
    }
   if (bound_ != NULL) {
       cout << "Defined on bound: " << endl;
      bound_->printInfo();
   }
    cout << endl;
    magnitude_->printInfo();
}

const Magnitude*
EMSource::getMagnitude() const {
    return magnitude_;
}

void
EMSource::applyGeometricScalingFactor(const double factor) {
    if (bound_ != NULL) {
        bound_->scale(factor);
    }
}

Condition::Type EMSource::getConditionType() const {
   return Condition::emSource;
}
