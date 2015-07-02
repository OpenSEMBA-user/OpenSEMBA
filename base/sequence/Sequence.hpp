/*
 * Sequence.hpp
 *
 *  Created on: 22/6/2015
 *      Author: Daniel
 */

#include "Sequence.h"

template<typename T>
Sequence<T>::Sequence() {

}

template<typename T>
Sequence<T>::~Sequence() {

}

template<typename T>
bool Sequence<T>::empty() const {
    return (size() == 0);
}

template<typename T>
typename Reference<T>::type Sequence<T>::operator()(UInt pos) {
    return get(pos);
}

template<typename T>
typename ConstReference<T>::type Sequence<T>::operator()(UInt pos) const {
    return get(pos);
}
