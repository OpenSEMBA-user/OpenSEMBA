/*
 * Quad.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#include "Quad.h"

template<class T>
Quad<T>::Quad(const LayerId layerId,
              const MatId   matId)
:   Surface<T>(layerId, matId) {

}

template<class T>
Quad<T>::Quad(const ElementId id,
              const LayerId layerId,
              const MatId   matId)
:   Surface<T>(id, layerId, matId) {

}

template<class T>
Quad<T>::Quad(const Quad<T>& rhs)
:   Surface<T>(rhs) {

}

template<class T>
Quad<T>::~Quad() {

}
