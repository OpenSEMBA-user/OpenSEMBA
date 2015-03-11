/*
 * Quad4.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#include "Quad4.h"

template<class T>
Quad4<T>::Quad4() {

}

template<class T>
Quad4<T>::Quad4(const CoordinateGroup<>& cG,
                const ElementId id,
                const CoordinateId vId[4],
                const LayerId layerId,
                const MatId   matId)
:   Quad<T>(id, layerId, matId) {
    
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = cG.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Quad4<T>::Quad4(): "
                 << "Coordinate in new CoordinateGroup inexistent"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        if (!coord->is< Coordinate<T,3> >()) {
            cerr << "ERROR @ Quad4<T>::Quad4(): "
                 << "Coordinate in new CoordinateGroup is not a valid Coordinate"
                 << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        v_[i] = coord->castTo< Coordinate<T,3> >();
	}
	// TODO Normals are not handled.
	check();
}

template<class T>
Quad4<T>::Quad4(const Quad4<T>& rhs)
:   Quad<T>(rhs) {
    
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

template<class T>
Quad4<T>::~Quad4() {
    
}

template<class T>
ClassBase* Quad4<T>::clone() const {
    return new Quad4<T>(*this);
}

template<class T>
const Coordinate<T,3>* Quad4<T>::getVertex(const UInt i) const {
	return v_[i];
}

template<class T>
const Coordinate<T,3>* Quad4<T>::getSideV(const UInt f,
                               const UInt i) const {
	assert(f < this->numberOfFaces());
	assert(i < numberOfSideCoordinates());
	return v_[(f + i) % 4];
}

template<class T>
const Coordinate<T,3>* Quad4<T>::getSideVertex(const UInt f,
                                    const UInt i) const {
	assert(f < this->numberOfFaces());
	assert(i < this->numberOfSideVertices());
	return v_[(f + i) % 4];
}

template<class T>
void Quad4<T>::setV(const UInt i, const Coordinate<T,3>* coord) {
    v_[i] = coord;
}

template<class T>
void Quad4<T>::printInfo() const {
	cout << "--- Quad4 info ---" << endl;
	Quad<T>::printInfo();
}

template<class T>
void Quad4<T>::check() const {
	// TODO Auto-generated
}
