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
   assert(false);
   return string();
}

string
Layer::getChildName() const {
   assert(false);
   return string();
}

const string&
Layer::getName() const {
    return name_;
}

void
Layer::printInfo() const {
    cout<< "Layer. Id: " << getId() << " Name: " << getName() << endl;
}
