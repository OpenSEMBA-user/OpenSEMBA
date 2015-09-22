/*
 * ClassGroup.hpp
 *
 *  Created on: 2/4/2015
 *      Author: Daniel
 */

#include "ClassGroupBase.h"

template<class T>
ClassGroupBase<T>::ErrorEmpty::ErrorEmpty(const std::string& func)
:   Error(func + std::string(" has no elements")) {

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
bool ClassGroupBase<T>::check(const std::string& func) const {
    if (this->elems().empty()) {
        throw ErrorEmpty(func);
    }
    return true;
}
