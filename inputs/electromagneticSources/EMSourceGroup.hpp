/*
 * EMSourceGroup.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

template<typename C>
EMSourceGroup<C>::EMSourceGroup() {

}


template<typename C>
UInt
EMSourceGroup<C>::countWithType(const ElementBase::Type type) const {
	UInt res = 0;
	for (UInt i = 0; i < this->size(); i++) {
		if (this->element_[i]->getElementType() == type) {
			res++;
		}
	}
	return res;
}

template<typename C>
const EMSource*
EMSourceGroup<C>::getWithType(const UInt j,
                              const ElementBase::Type param) const {
	assert(j < countWithType(param));
	UInt count = 0;
	for (UInt i = 0; i < this->size(); i++) {
		if (this->element_[i]->getElementType() == param) {
			if (count == j) {
				return this->element_[i];
			} else {
				count++;
			}
		}
	}
	cerr<< "ERROR @ EMSourceGroup: "
		<< "EMSource was not found in group." << endl;
	assert(false);
	return NULL;
}

template<typename C>
void
EMSourceGroup<C>::printInfo() const {
	cout << " --- EMSourceGroup info --- " << endl;
	for (UInt i = 0; i < this->size(); i++) {
		this->element_[i]->printInfo();
	}
}
