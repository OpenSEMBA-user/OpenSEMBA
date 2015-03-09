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

template<typename L>
LayerGroup<L>::LayerGroup(const vector<L*>& layers)
:   GroupWithIdBase<L, LayerId>(layers) {

}

template<typename L>
LayerGroup<L>::LayerGroup(const GroupBase<L>& rhs)
:   GroupWithIdBase<L, LayerId>(rhs) {

}

template<typename L>
LayerGroup<L>::~LayerGroup() {

}

template<typename L>
LayerGroup<L>& LayerGroup<L>::operator=(const GroupBase<L>& rhs) {
    if (this == &rhs) {
        return *this;
    }

    GroupWithIdBase<L, LayerId>::operator=(rhs);

    return *this;
}

template<typename L>
vector<LayerId> LayerGroup<L>::getIds() const {
    vector<LayerId> res;
    res.resize(this->size());
    for(unsigned i = 0; i < this->size(); i++) {
        res[i] = this->element_[i]->getId();
    }
    return res;
}

template<typename L>
void LayerGroup<L>::printInfo() const {
    cout<< "--- Layers info ---" << endl;
    map<uint, const Layer*>::const_iterator it;
    for (it=layers_.begin(); it != layers_.end(); ++it) {
        it->second->printInfo();
    }
    cout<< "--- End of Layers ---" << endl;
}
