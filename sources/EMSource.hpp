/*
 * EMSource.hpp
 *
 *  Created on: 2/4/2015
 *      Author: Daniel
 */

#include "../sources/EMSource.h"

template <class T>
void EMSource<T>::set(const GroupElements<const Elem>& elems) {
    GroupElements<const T>::operator=(elems);
}

template <class T>
void EMSource<T>::add(const GroupElements<const Elem>& elems) {
    GroupElements<const T>::add(elems);
}

template<class T>
bool EMSource<T>::isSimilar(const EMSourceBase& rhs) const {
    if (!this->hasSameProperties(rhs)) {
        return false;
    }
    UInt sizeLhs = this->elems().size();
    UInt sizeRhs = rhs.elems().size();
    if ((sizeLhs == 0) && (sizeRhs == 0)) {
        return true;
    } else if ((sizeLhs == 0) || (sizeRhs == 0)) {
        return false;
    }
    return true;
}

template<class T>
void EMSource<T>::printInfo() const {
    EMSourceBase::printInfo();
//    this->GroupElements<const T>::printInfo();
}
