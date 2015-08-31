/*
 * Polyline.cpp
 *
 *  Created on: Apr 15, 2015
 *      Author: damarro
 */

#include "Polyline.h"

template<class T>
Polyline<T>::Polyline() {

}

template<class T>
Polyline<T>::Polyline(const ElementId id,
                      const vector<const Coordinate<T,3>*>& v,
                      const LayerId layerId,
                      const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {
    v_ = v;
}

template<class T>
Polyline<T>::Polyline(const Polyline<T>& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {
    v_ = rhs.v_;
}

template<class T>
Polyline<T>::~Polyline() {

}

template<class T>
const Coordinate<T,3>* Polyline<T>::getSideV(const UInt f, const UInt i) const {
    if (f == 0) {
        return v_.front();
    }
    return v_.back();
}

template<class T>
const Coordinate<T,3>* Polyline<T>::getVertex(const UInt i) const {
    if (i == 0) {
        return v_.front();
    }
    return v_.back();
}

template<class T>
const Coordinate<T,3>* Polyline<T>::getSideVertex(const UInt f,
                                                  const UInt i) const {
    if (f == 0) {
        return v_.front();
    }
    return v_.back();
}

template<class T>
void Polyline<T>::setV(const UInt i, const Coordinate<T,3>* coord) {

    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

template<class T>
void Polyline<T>::printInfo() const {
    cout << "--- Polyline info ---" << endl;
    Line<T>::printInfo();
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

template class Polyline<Real>;
template class Polyline<Int >;
