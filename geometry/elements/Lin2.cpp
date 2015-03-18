/*
 * Lin2.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef LIN2_H_
#include "Lin2.h"
#endif

template<class T>
const SimplexLin<1> Lin2<T>::lin;

template<class T>
Lin2<T>::Lin2() {

}

template<class T>
Lin2<T>::Lin2(const CoordinateGroup<Coordinate<T,3> >& coordGr,
              const ElementId id,
              const CoordinateId vId[2],
              const LayerId layerId,
              const MatId   matId)
:   Elem(id, layerId, matId) {
    
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
	    v_[i] = coordGr.getPtrToId(vId[i]);
	}
}

template<class T>
Lin2<T>::Lin2(const ElementId id,
              const Coordinate<T,3>* v[2],
              const LayerId layerId,
              const MatId   matId)
:   Elem(id, layerId, matId) {
    
	for (UInt i = 0; i < lin.np; i++) {
		v_[i] = v[i];
	}
}

template<class T>
Lin2<T>::Lin2(CoordinateGroup<Coordinate<T,3> >& cG,
              const ElementId id,
              const Box<T,3>& box,
              const LayerId layerId,
              const MatId   matId)
:   Elem(id, layerId, matId) {

    if(!box.isLine()) {
        cerr << endl << "ERROR @ Lin2::Lin2(): "
                     << "Box is not a Line" << endl;
        assert(false);
        exit(EXIT_FAILURE);
    }
    vector<CartesianVector<T,3> > pos = box.getPos();
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = cG.get(pos[i]);
        if (v_[i] == NULL) {
            v_[i] = cG.add(pos[i]);
        }
    }
}

template<class T>
Lin2<T>::Lin2(const Lin2<T>& rhs)
:   Elem(rhs) {
    
    for (UInt i = 0; i < lin.np; i++) {
		v_[i] = rhs.v_[i];
	}
}

template<class T>
Lin2<T>::~Lin2() {

}

template<class T>
ClassBase* Lin2<T>::clone() const {
    return new Lin2<T>(*this);
}

template<class T>
const Coordinate<T,3>* Lin2<T>::getSideV(const UInt f, const UInt i) const {
    return v_[i];
}

template<class T>
const Coordinate<T,3>* Lin2<T>::getVertex(const UInt i) const {
	return v_[i];
}

template<class T>
const Coordinate<T,3>* Lin2<T>::getSideVertex(const UInt f,
                                              const UInt i) const {
    return v_[i];
}

template<class T>
void Lin2<T>::setV(const UInt i, const Coordinate<T,3>* coord) {

    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

template<class T>
void Lin2<T>::printInfo() const {
    cout << "--- Lin2 info ---" << endl;
    cout << "Id: " << this->getId() << endl;
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
        cout << endl;
    }
}

template class Lin2<Real>;
template class Lin2<Int >;
