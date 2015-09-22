/*
 * OutRqBulkCurrent.cpp
 *
 *  Created on: 20 de ago. de 2015
 *      Author: Daniel
 */

#include <outRq/OutRqBulkCurrent.h>

OutRqBulkCurrent::OutRqBulkCurrent(const Domain& domain,
                                   const string& name,
                                   const GroupElements<Elem>& elem,
                                   const CartesianAxis& dir,
                                   const UInt& skip)
:   Domain(domain),
    OutRqBase(bulkCurrentElectric, name),
    GroupElements<const Elem>(elem) {

    dir_ = dir;
    skip_ = skip;
}

OutRqBulkCurrent::OutRqBulkCurrent(const OutRqBulkCurrent& rhs)
:   Domain(rhs),
    OutRqBase(rhs),
    GroupElements<const Elem>(rhs) {

    dir_ = rhs.dir_;
    skip_ = rhs.skip_;
}

OutRqBulkCurrent::~OutRqBulkCurrent() {

}

bool OutRqBulkCurrent::hasSameProperties(const OutRqBase& rhs) const {
    if(!OutRqBase::hasSameProperties(rhs)) {
        return false;
    }
    const OutRqBulkCurrent* rhsPtr = rhs.castTo<OutRqBulkCurrent>();
    bool hasSameProperties = true;
    hasSameProperties &= dir_ == rhsPtr->dir_;
    hasSameProperties &= skip_ == rhsPtr->skip_;
    return hasSameProperties;
}

CartesianAxis OutRqBulkCurrent::getDir() const {
    return dir_;
}

UInt OutRqBulkCurrent::getSkip() const {
    return skip_;
}

