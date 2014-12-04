/*
 * LayerGroup.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#include "LayerGroup.h"

LayerGroup::LayerGroup() {
}

LayerGroup::~LayerGroup() {
}

void
LayerGroup::add(Layer* layer) {
    layers_.insert(pair<uint, const Layer*>(layer->getId(), layer));
}

void
LayerGroup::printInfo() const {
    cout<< "--- Layers info ---" << endl;
    map<uint, const Layer*>::const_iterator it;
    for (it=layers_.begin(); it != layers_.end(); ++it) {
        it->second->printInfo();
    }
    cout<< "--- End of Layers ---" << endl;
}
