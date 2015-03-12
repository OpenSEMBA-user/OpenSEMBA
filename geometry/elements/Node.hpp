
#include "Node.h"

template<class T>
Node<T>::Node() {

}

template<class T>
Node<T>::Node(const CoordinateGroup<>& coordGr,
              const ElementId id,
              const CoordinateId vId[1],
              const LayerId layerId,
              const MatId   matId)
:   Element<T>(id, layerId, matId) {
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
		const CoordinateBase* coord = coordGr.getPtrToId(vId[i]);
        if (coord == NULL) {
            cerr << "ERROR @ Node<T>::Node(): "
                 << "Coordinate in new CoordinateGroup inexistent"
                 << endl;
        }
        if (!coord->is< Coordinate<T,3> >()) {
            cerr << "ERROR @ Node<T>::Node(): "
                 << "Coordinate in new CoordinateGroup is not a valid Coordinate"
                 << endl;
        }
        v_[i] = coord->castTo< Coordinate<T,3> >();
	}
}

template<class T>
Node<T>::Node(const ElementId id,
              const Coordinate<T,3>* v[1],
              const LayerId layerId,
              const MatId   matId)
:   Element<T>(id, layerId, matId) {
    v_[0] = v[0];
}

template<class T>
Node<T>::Node(const Node<T>& rhs)
:   Element<T>(rhs) {
    	v_[0] = rhs.v_[0];
}

template<class T>
Node<T>::~Node() {

}

template<class T>
ClassBase* Node<T>::clone() const {
    return new Node<T>(*this);
}

template<class T>
const Coordinate<T,3>* Node<T>::getSideV(const UInt f, const UInt i) const {
    assert(f == 0 && i == 0);
    return v_[i];
}

template<class T>
const Coordinate<T,3>* Node<T>::getVertex(const UInt i) const {
    assert(i == 0);
	return v_[i];
}

template<class T>
const Coordinate<T,3>* Node<T>::getSideVertex(const UInt f, const UInt i) const {
    assert(f == 0 && i == 0);
    return v_[i];
}

template<class T>
void Node<T>::setV(const UInt i, const Coordinate<T,3>* coord) {
	assert(i < numberOfCoordinates());
	v_[i] = coord;
}

template<class T>
void Node<T>::printInfo() const {
	cout << "--- Node info ---" << endl;
	Element<T>::printInfo();
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
		v_[i]->printInfo();
		cout << endl;
	}
}
