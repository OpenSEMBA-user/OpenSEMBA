#ifndef OUTPUTREQUESTGROUP_H_
#	include "OutputRequestGroup.h"
#endif

OutputRequestGroup::OutputRequestGroup(
 const vector<OutputRequest>& in) {
	for (uint i = 0; i < in.size(); i++) {
		// Checks if exists similar output and combines elements if so.
		bool isSimilar = false;
		for (uint j = 0; j < size(); j++) {
			if (in[i].isSimilar(*element_[j])) {
				isSimilar = true;
				element_[j]->setAdditionalElems(in[i].getElem());
			}
		}
		if (!isSimilar) {
			add(new OutputRequest(in[i]));
		}
	}
}

uint
OutputRequestGroup::countWithType(const Element::Type param) const {
	uint res = 0;
	for (uint i = 0; i < size(); i++) {
		if (element_[i]->getElementType() == param) {
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
	for (uint i = 0; i < size(); i++) {
		if (element_[i]->getElementType() == param) {
			if (count == j) {
				return element_[i];
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

void
OutputRequestGroup::printInfo() const {
	cout<< " --- OutputRequestGroup info ---" << endl;
	for (unsigned int i = 0; i < element_.size(); i++) {
		element_[i]->printInfo();
	}
	cout<< " --- End of outputrequestGroup info ---" << endl;
}
