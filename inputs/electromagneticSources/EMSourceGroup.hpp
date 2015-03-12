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
void
EMSourceGroup<C>::printInfo() const {
	cout << " --- EMSourceGroup info --- " << endl;
	for (UInt i = 0; i < this->size(); i++) {
		this->element_[i]->printInfo();
	}
}
