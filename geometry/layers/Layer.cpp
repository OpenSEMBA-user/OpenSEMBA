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
    cout << toStr() << endl;
}

bool Layer::operator ==(const Layer& rhs) const {
    bool res = true;
    res &= ClassIdBase<LayerId>::operator==(rhs);
    res &= (name_ == rhs.name_);
    return res;
}

bool Layer::operator !=(const Layer& rhs) const {
    return !(*this == rhs);
}

string Layer::toStr() const {
    stringstream ss;
    ss << "Layer. Id: " << getId() << " Name: " << getName();
    return ss.str();
}
