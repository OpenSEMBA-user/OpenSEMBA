/*
 * Vector.hpp
 *
 *  Created on: 23/6/2015
 *      Author: Daniel
 */

#include "Vector.h"

template<typename T>
Vector<T>::Vector() {

}

template<typename T>
Vector<T>::Vector(const T& elems) {
    element_.reserve(1);
    element_.push_back(elems);
}

template<typename T>
Vector<T>::Vector(const std::vector<T>& elems) {
    element_ = elems;
}

template<typename T>
Vector<T>::Vector(const Vector& rhs) {
    element_ = rhs.element_;
}

template<typename T>
Vector<T>::~Vector() {

}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs) {
    if (this == &rhs) {
        return *this;
    }
    element_ = rhs.element_;
    return *this;
}

template<typename T>
typename Reference<T>::type Vector<T>::get(const UInt i) {
    return element_[i];
}

template<typename T>
typename ConstReference<T>::type Vector<T>::get(const UInt i) const {
    return element_[i];
}

template<typename T>
Vector<T> Vector<T>::add(const Vector& rhs) {
    element_.insert(element_.end(), rhs.element_.begin(), rhs.element_.end());
    return rhs;
}
