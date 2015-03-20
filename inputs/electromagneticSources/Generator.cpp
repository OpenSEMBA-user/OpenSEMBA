/*
 * Generator.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: luis
 */

#include "Generator.h"

Generator::Generator() {
    type_ = current;
    hardness_ = soft;
}

Generator::Generator(const Magnitude* magnitude,
                     const ElementsGroup<Node<> >& elems,
                     const Type& generatorType,
                     const Hardness& hardness)
:   EMSource<>(magnitude),
    ElementsGroup<Node<> >(elems) {
    hardness_ = hardness;
    type_ = generatorType;
}

Generator::Generator(const Generator& rhs)
:   EMSource<>(rhs),
    ElementsGroup<Node<> >(rhs) {

    hardness_ = rhs.hardness_;
    type_ = rhs.type_;
}

Generator::~Generator() {

}

ClassBase* Generator::clone() const {
    return new Generator(*this);
}

Generator::Type
Generator::getType() const {
    return type_;
}

void
Generator::printInfo() const {
    cout<< " --- Generator info --- " << endl;
    EMSource::printInfo();
    cout<< "Type: " << getTypeStr() << endl;
}

string
Generator::getTypeStr() const {
    switch (type_) {
    case voltage:
        return "Voltage";
    default:
        return "Current";
    }
}
