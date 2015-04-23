/*
 * EMSource.hpp
 *
 *  Created on: 2/4/2015
 *      Author: Daniel
 */

#include "EMSource.h"

template <class T>
void EMSource<T>::set(const ElementsGroup<const Elem>& elems) {
    ElementsGroup<const T>::operator=(elems);
}

template<class T>
bool EMSource<T>::check() const {
    if (this->size() == 0) {
        cerr << endl << "ERROR @ EMSource: "
                << "EMSource has no elements." << endl;
        this->printInfo();
        return false;
    }
    return true;
}

template <class T>
void EMSource<T>::add(const ElementsGroup<const Elem>& elems) {
    ElementsGroup<const T>::add(elems);
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
    this->ElementsGroup<const T>::printInfo();
}
