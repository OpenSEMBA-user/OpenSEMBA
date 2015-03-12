template<typename C>
OutRqGroup<C>::OutRqGroup() {
}

template<typename C>
OutRqGroup<C>::OutRqGroup(
 const vector<OutRq>& in) {
	for (UInt i = 0; i < in.size(); i++) {
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
void OutRqGroup<C>::add (OutRq* in) {
    bool foundSimilar = false;
    for (UInt j = 0; j < size(); j++) {
        if (in->isSimilar(*element_[j])) {
            foundSimilar = true;
            element_[j]->setAdditionalElems(in->getElem());
            delete in;
        }
    }
    if (!foundSimilar) {
        Group<C>::add(in);
    }
}

template<typename C>
void OutRqGroup<C>::printInfo() const {
	cout<< " --- OutputRequestGroup info ---" << endl;
	for (UInt i = 0; i < element_.size(); i++) {
		element_[i]->printInfo();
	}
	cout<< " --- End of outputrequestGroup info ---" << endl;
}
