/*
 * GroupLayers.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#include "GroupLayers.h"

template<typename L>
const L* GroupLayers<L>::get(const string name) const {
    for (UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->getName() == name) {
            return this->element_[i];
        }
    }
    return NULL;
}

template<typename L>
void GroupLayers<L>::printInfo() const {
    cout<< "--- GroupLayers info ---" << endl;
    Group<L>::printInfo();
}
