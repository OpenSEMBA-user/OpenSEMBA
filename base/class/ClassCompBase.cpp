/*
 * ClassCompBase.cpp
 *
 *  Created on: 1/4/2015
 *      Author: Daniel
 */

#include "ClassCompBase.h"

bool ClassCompBase::isSameType(const ClassCompBase& rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        return true;
    }
    return false;
}

bool ClassCompBase::isSimilar(const ClassCompBase& rhs) const {
    return isSameType(rhs);
}

bool ClassCompBase::operator==(const ClassCompBase& rhs) const {
    return (!(*this < rhs) && !(rhs < *this));
}

bool ClassCompBase::operator!=(const ClassCompBase& rhs) const {
    return !(*this == rhs);
}

bool ClassCompBase::operator< (const ClassCompBase& rhs) const {
    if (string(typeid(*this).name()) < string(typeid(rhs).name())) {
        return true;
    }
    return false;
}

bool ClassCompBase::operator<=(const ClassCompBase& rhs) const {
    return ((*this < rhs) || (*this == rhs));
}

bool ClassCompBase::operator> (const ClassCompBase& rhs) const {
    return (rhs < *this);
}

bool ClassCompBase::operator>=(const ClassCompBase& rhs) const {
    return (rhs <= *this);
}
