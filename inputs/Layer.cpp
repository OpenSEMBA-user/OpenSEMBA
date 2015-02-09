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

uint
Layer::getId() const {
    return id_;
}

string
Layer::getParentName() const {
#   warning "Layer parent name not implemented."
}

string
Layer::getChildName() const {
#   warning "Layer parent name not implemented."
}

const string&
Layer::getName() const {
    return name_;
}

void
Layer::printInfo() const {
    cout<< "Layer. Id: " << getId() << " Name: " << getName() << endl;
}
