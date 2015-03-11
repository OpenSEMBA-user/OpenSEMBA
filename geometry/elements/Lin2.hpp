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
Lin2<T>::Lin2(const CoordinateGroup<>& coordGr,
              const ElementId id,
              const CoordinateId vId[2],
              const LayerId layerId,
              const MatId   matId)
:   Line<T>(id, layerId, matId) {
    
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Lin2<T>::Lin2(): "
                 << "Coordinate in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        if (!coord->is< Coordinate<T,3> >()) {
            cerr << "ERROR @ Lin2<T>::Lin2(): "
                 << "Coordinate in new CoordinateGroup is not a valid Coordinate"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        v_[i] = coord->castTo< Coordinate<T,3> >();
	}
}

template<class T>
Lin2<T>::Lin2(const ElementId id,
              const Coordinate<T,3>* v[2],
              const LayerId layerId,
              const MatId   matId)
:   Line<T>(id, layerId, matId) {
    
	for (UInt i = 0; i < lin.np; i++) {
		v_[i] = v[i];
	}
}

template<class T>
Lin2<T>::Lin2(const Lin2<T>& rhs)
:   Line<T>(rhs) {
    
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
const Coordinate<T,3>* Lin2<T>::getSideVertex(const UInt f, const UInt i) const {
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
