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
uint
EMSourceGroup<C>::countWithType(const Element::Type type) const {
	uint res = 0;
	for (uint i = 0; i < this->size(); i++) {
		if (this->element_[i]->getElementType() == type) {
			res++;
		}
	}
	return res;
}

template<typename C>
const EMSource*
EMSourceGroup<C>::getWithType(
 const uint j,
 const Element::Type param) const {
	assert(j < countWithType(param));
	uint count = 0;
	for (uint i = 0; i < this->size(); i++) {
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
	for (unsigned int i = 0; i < this->size(); i++) {
		this->element_[i]->printInfo();
	}
}
