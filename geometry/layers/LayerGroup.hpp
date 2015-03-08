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
:   GroupId<L, LayerId>(layers) {

}

template<typename L>
LayerGroup<L>::LayerGroup(const Group<L>& rhs)
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

template<typename L>
void LayerGroup<L>::printInfo() const {
    cout<< "--- Layers info ---" << endl;
    map<uint, const Layer*>::const_iterator it;
    for (it=layers_.begin(); it != layers_.end(); ++it) {
        it->second->printInfo();
    }
    cout<< "--- End of Layers ---" << endl;
}
