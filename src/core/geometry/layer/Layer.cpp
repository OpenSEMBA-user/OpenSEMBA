

#include "Layer.h"

#include <cassert>
#include <iostream>
#include <sstream>

namespace SEMBA {
namespace Geometry {
namespace Layer {

Layer::Layer() {

}

Layer::Layer(const Id id, const std::string& name)
:   Identifiable<Id>(id) {
    name_ = spaceToUnderscore(name);
}

Layer::Layer(const std::string& name) {
    name_ = spaceToUnderscore(name);
}

Layer::Layer(const Layer& rhs)
:   Identifiable<Id>(rhs) {
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

std::string Layer::getName() const {
    return name_;
}

std::string Layer::getParentName() const {
   assert(false);
   return std::string();
}

std::string Layer::getChildName() const {
   assert(false);
   return std::string();
}

void Layer::printInfo() const {
    std::cout << toStr() << std::endl;
}

std::string Layer::toStr() const {
    std::stringstream ss;
    ss << "Layer. Id: " << getId() << " Name: " << getName();
    return ss.str();
}

std::string Layer::spaceToUnderscore(std::string rhs) {
    std::string str = rhs;
    for(std::string::iterator it = str.begin(); it != str.end(); ++it) {
        if(*it == ' ') {
            *it = '_';
        }
    }
    return str;
}

} /* namespace Layer */
} /* namespace Geometry */
} /* namespace SEMBA */
