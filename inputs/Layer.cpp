/*
 * Layer.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#include "Layer.h"

Layer::Layer() {
    id_ = 0;
}

Layer::Layer(uint id, const string& name) {
    id_ = id;
    name_ = name;
}

Layer::~Layer() {
    // TODO Auto-generated destructor stub
}

