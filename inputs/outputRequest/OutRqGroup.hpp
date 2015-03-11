template<typename C>
OutRqGroup<C>::OutRqGroup() {
}

template<typename C>
OutRqGroup<C>::OutRqGroup(
 const vector<OutRq>& in) {
	for (UInt i = 0; i < in.size(); i++) {
		// Checks if exists similar output and combines elements if so.
		bool isSimilar = false;
		for (UInt j = 0; j < size(); j++) {
			if (in[i].isSimilar(*element_[j])) {
				isSimilar = true;
				element_[j]->setAdditionalElems(in[i].getElem());
			}
		}
		if (!isSimilar) {
			add(new OutRq(in[i]));
		}
	}
}

template<typename C>
UInt
OutRqGroup<C>::countWithType(const ElementBase::Type param) const {
	UInt res = 0;
	for (UInt i = 0; i < size(); i++) {
		if (element_[i]->getElementType() == param) {
			res++;
		}
	}
	return res;
}

template<typename C>
const OutRq*
OutRqGroup<C>::getWithType(
 const UInt j,
 const ElementBase::Type param) const {
	assert(j < countWithType(param));
	UInt count = 0;
	for (UInt i = 0; i < size(); i++) {
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

template<typename C>
void
OutRqGroup<C>::printInfo() const {
	cout<< " --- OutputRequestGroup info ---" << endl;
	for (UInt i = 0; i < element_.size(); i++) {
		element_[i]->printInfo();
	}
	cout<< " --- End of outputrequestGroup info ---" << endl;
}
