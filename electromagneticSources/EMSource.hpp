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

template <class T>
void EMSource<T>::add(const ElementsGroup<const Elem>& elems) {
    ElementsGroup<const T>::add(elems);
}
