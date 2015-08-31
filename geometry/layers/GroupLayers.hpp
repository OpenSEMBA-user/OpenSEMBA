/*
 * GroupLayers.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#include "GroupLayers.h"

template<typename L>
GroupLayers<L>& GroupLayers<L>::operator=(VectorPtr<L>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<L, LayerId>::operator=(rhs);
    return *this;
}

template<typename L>
GroupLayers<L>& GroupLayers<L>::operator=(VectorPtr<L>&& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<L, LayerId>::operator=(std::move(rhs));
    return *this;
}

template<typename L>
const L* GroupLayers<L>::getName(const string name) const {
    for (UInt i = 0; i < this->size(); i++) {
        if (this->get(i)->getName() == name) {
            return this->get(i);
        }
    }
    return NULL;
}

template<typename L>
void GroupLayers<L>::printInfo() const {
    cout<< "--- GroupLayers info ---" << endl;
    GroupId<L, LayerId>::printInfo();
}
