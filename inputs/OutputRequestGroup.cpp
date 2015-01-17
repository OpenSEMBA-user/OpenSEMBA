#ifndef OUTPUTREQUESTGROUP_H_
#	include "OutputRequestGroup.h"
#endif

OutputRequestGroup::OutputRequestGroup() {

}

OutputRequestGroup::OutputRequestGroup(
 const vector<OutputRequest>& in) {
	oR_.clear();
	for (uint i = 0; i < in.size(); i++) {
		// Checks if exists similar output and combines elements if so.
		bool isSimilar = false;
		for (uint j = 0; j < oR_.size(); j++) {
			if (in[i].isSimilar(oR_[j])) {
				isSimilar = true;
				oR_[j].setAdditionalElems(in[i].getElem());
			}
		}
		if (!isSimilar) {
			oR_.push_back(in[i]);
		}
	}
}

uint
OutputRequestGroup::countWithType(const Element::Type param) const {
	uint res = 0;
	for (uint i = 0; i < oR_.size(); i++) {
		if (oR_[i].getElementType() == param) {
			res++;
		}
	}
	return res;
}

const OutputRequest*
OutputRequestGroup::getWithType(
 const uint j,
 const Element::Type param) const {
	assert(j < countWithType(param));
	uint count = 0;
	for (uint i = 0; i < oR_.size(); i++) {
		if (oR_[i].getElementType() == param) {
			if (count == j) {
				return &oR_[i];
			} else {
				count++;
			}
		}
	}
	cerr<< "ERROR @ OutputRequestGroup: "
		<< "Output Request was not found in group." << endl;
	assert(false);
	return NULL;
}

const OutputRequest* OutputRequestGroup::get(const uint i) const {
	return &oR_[i];
}

void
OutputRequestGroup::printInfo() const {
	cout<< " --- OutputRequestGroup info ---" << endl;
	for (unsigned int i = 0; i < oR_.size(); i++) {
		oR_[i].printInfo();
	}
	cout<< " --- End of outputrequestGroup info ---" << endl;
}

void
OutputRequestGroup::applyGeometricScalingFactor(
 const double scalingFactor) {
}

uint OutputRequestGroup::count() const {
	return oR_.size();
}
