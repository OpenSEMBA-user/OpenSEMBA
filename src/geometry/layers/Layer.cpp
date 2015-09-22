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

bool Layer::operator ==(const Layer& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    bool res = true;
    res &= (this->getId() == rhs.getId());
    res &= (this->getName() == rhs.getName());
    return res;
}

bool Layer::operator !=(const Layer& rhs) const {
    return !(*this == rhs);
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

string Layer::toStr() const {
    stringstream ss;
    ss << "Layer. Id: " << getId() << " Name: " << getName();
    return ss.str();
}
