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

Layer::~Layer() {
    // TODO Auto-generated destructor stub
}

ClassBase* Layer::clone() const {
   return new Layer(*this);
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
