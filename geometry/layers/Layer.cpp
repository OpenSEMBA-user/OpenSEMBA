/*
 * Layer.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: luis
 */

#include "Layer.h"

Layer::Layer() {

}

Layer::Layer(const LayerId id, const string& name)
:   ClassIdBase<LayerId>(id) {
    name_ = name;
}

Layer::Layer(const string& name) {
    name_ = name;
}

Layer::Layer(const Layer& rhs)
:   ClassIdBase<LayerId>(rhs) {
    name_ = rhs.name_;
}

Layer::~Layer() {

}

string Layer::getName() const {
    return name_;
}

string Layer::getParentName() const {
   assert(false);
   return string();
}

string Layer::getChildName() const {
   assert(false);
   return string();
}

void Layer::printInfo() const {
    cout<< "Layer. Id: " << getId() << " Name: " << getName() << endl;
}
