#ifndef OUTPUTREQUESTGROUP_H_
#	include "OutputRequestGroup.h"
#endif

OutputRequestGroup::OutputRequestGroup() {

}

OutputRequestGroup::OutputRequestGroup(
 const vector<OutputRequest>& in) {
	outputRequest.clear();
	for (uint i = 0; i < in.size(); i++) {
		// Checks if exists similar output and combines elements if so.
		bool isSimilar = false;
		for (uint j = 0; j < outputRequest.size(); j++) {
			if (in[i].isSimilar(outputRequest[j])) {
				isSimilar = true;
				outputRequest[j].setAdditionalElems(in[i].getElem());
			}
		}
		if (!isSimilar) {
			outputRequest.push_back(in[i]);
		}
	}
}

uint
OutputRequestGroup::countWithType(const Element::Type param) const {
	uint res = 0;
	for (uint i = 0; i < outputRequest.size(); i++) {
		if (outputRequest[i].getElementType() == param) {
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
	for (uint i = 0; i < outputRequest.size(); i++) {
		if (outputRequest[i].getElementType() == param) {
			if (count == j) {
				return &outputRequest[i];
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
	for (unsigned int i = 0; i < outputRequest.size(); i++) {
		outputRequest[i].printInfo();
	}
	cout<< " --- End of outputrequestGroup info ---" << endl;
}

void
OutputRequestGroup::applyGeometricScalingFactor(
 const double scalingFactor) {
}

