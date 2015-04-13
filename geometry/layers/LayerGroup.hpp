/*
 * LayerGroup.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#include "LayerGroup.h"

template<typename L>
const L* LayerGroup<L>::get(const string name) const {
    for (UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->getName() == name) {
            return this->element_[i];
        }
    }
    return NULL;
}

template<typename L>
void LayerGroup<L>::printInfo() const {
    cout<< "--- Layers info ---" << endl;
    map<UInt, const Layer*>::const_iterator it;
    Group<L>::printInfo();
}
