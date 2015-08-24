/*
 * Array.hpp
 *
 *  Created on: 24/6/2015
 *      Author: Daniel
 */

#include "Array.h"

template<typename T, Int D>
Array<T,D>::Array() {

}

template<typename T, Int D>
Array<T,D>::Array(const T& elem) {
    for (Int d = 0; d < D; d++) {
        element_[d] = elem;
    }
}

template<typename T, Int D>
Array<T,D>::Array(const std::array<T,D>& elems) {
    element_ = elems;
}

template<typename T, Int D>
Array<T,D>::Array(const Array& rhs) {
    element_ = rhs.element_;
}

template<typename T, Int D>
Array<T,D>::~Array() {

}

template<typename T, Int D>
Array<T,D>& Array<T,D>::operator=(const Array& rhs) {
    if (this == &rhs) {
        return *this;
    }
    element_ = rhs.element_;
    return *this;
}

template<typename T, Int D>
typename Reference<T>::type Array<T,D>::get(const UInt i) {
    return element_[i];
}

template<typename T, Int D>
typename ConstReference<T>::type Array<T,D>::get(const UInt i) const {
    return element_[i];
}
