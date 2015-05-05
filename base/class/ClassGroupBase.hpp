/*
 * ClassGroup.hpp
 *
 *  Created on: 2/4/2015
 *      Author: Daniel
 */

#include "ClassGroupBase.h"

template<class T>
ClassGroupBase<T>::ErrorEmpty::ErrorEmpty(const string& func)
:   Error(func + string(" has no elements")) {

}

template<class T>
ClassGroupBase<T>::ErrorEmpty::~ErrorEmpty() throw() {

}

template<typename T>
ClassGroupBase<T>::ClassGroupBase() {

}

template<typename T>
ClassGroupBase<T>::~ClassGroupBase() {

}

template<class T>
bool ClassGroupBase<T>::check(const string& func) const {
    if (this->elems().empty()) {
        throw ErrorEmpty(func);
    }
    return true;
}
