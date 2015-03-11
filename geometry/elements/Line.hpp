/*
 * Line.cpp
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#include "Line.h"

template<class T>
Line<T>::Line(const LayerId layerId,
              const MatId   matId)
:   Element<T>(layerId, matId) {

}

template<class T>
Line<T>::Line(const ElementId id,
              const LayerId layerId,
              const MatId   matId)
:   Element<T>(id, layerId, matId) {

}

template<class T>
Line<T>::Line(const Line<T>& rhs)
:   Element<T>(rhs) {

}

template<class T>
Line<T>::~Line() {

}
