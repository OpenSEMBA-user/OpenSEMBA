/*
 * LayerGroup.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#include "LayerGroup.h"

template<typename L>
LayerGroup<L>::LayerGroup() {

}

template<typename L> template<typename L2>
LayerGroup<L>::LayerGroup(const vector<L2*>& layers)
:   GroupId<L, LayerId>(layers) {

}

template<typename L>
LayerGroup<L>::LayerGroup(const Group<L>& rhs)
:   GroupId<L, LayerId>(rhs) {

}

template<typename L> template<typename L2>
LayerGroup<L>::LayerGroup(const Group<L2>& rhs)
:   GroupId<L, LayerId>(rhs) {

}

template<typename L>
LayerGroup<L>::~LayerGroup() {

}

template<typename L>
LayerGroup<L>& LayerGroup<L>::operator=(const Group<L>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    GroupId<L, LayerId>::operator=(rhs);

    return *this;
}

template<typename L> template<typename L2>
LayerGroup<L>& LayerGroup<L>::operator=(const Group<L2>& rhs) {
    GroupId<L, LayerId>::operator=(rhs);

    return *this;
}

template<typename L>
LayerGroup<L> LayerGroup<L>::get(const LayerId& id) const {
    return GroupId<L, LayerId>::get(id);
}

template<typename L>
LayerGroup<L> LayerGroup<L>::get(
        const vector<LayerId>& ids) const {
    return GroupId<L, LayerId>::get(ids);
}

template<typename L>
const L* LayerGroup<L>::get(const string name) const {
    for (UInt i = 0; i < this->size(); i++) {
        if (this->element_[i]->getName() == name) {
            return this->element_[i];
        }
    }
    return NULL;
}

template<typename L>
void LayerGroup<L>::printInfo() const {
    cout<< "--- Layers info ---" << endl;
    map<UInt, const Layer*>::const_iterator it;
    for (UInt i = 0; i < this->size(); i++) {
        this->element_[i]->printInfo();
    }
    cout<< "--- End of Layers ---" << endl;
}
