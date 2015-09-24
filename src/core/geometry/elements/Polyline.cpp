// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
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
