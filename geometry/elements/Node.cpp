#include "Node.h"

template<class T>
Node<T>::Node() {

}

template<class T>
Node<T>::Node(const CoordinateGroup< Coordinate<T,3> >& coordGr,
              const ElementId id,
              const CoordinateId vId[1],
              const LayerId layerId,
              const MatId   matId)
:   Elem(id, layerId, matId) {
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = coordGr.getPtrToId(vId[i]);
	}
}

template<class T>
Node<T>::Node(const ElementId id,
              const Coordinate<T,3>* v[1],
              const LayerId layerId,
              const MatId   matId)
:   Elem(id, layerId, matId) {
    v_[0] = v[0];
}

template<class T>
Node<T>::Node(const Node<T>& rhs)
:   Elem(rhs) {
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
ElemI* Node<T>::toStructured(CoordinateGroup<CoordI3>& cG,
                             const Grid3& grid) const {
    if (this->template is<ElemI>()) {
        return NULL;
    }
    CVecI3 cell;
    const CoordI3* coord;
    CoordinateId* vIds = new CoordinateId[this->numberOfCoordinates()];
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        if (!grid.isCell(*this->getV(i))) {
            cerr << endl << "ERROR @ Element::toStructured(): "
                 << "Element with vertex not Structured" << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        cell  = grid.getCell(*this->getV(i));
        coord = cG.get(cell);
        if (coord == NULL) {
            coord = cG.add(cell);
        }
        vIds[i] = coord->getId();
    }
    ElemI* res =  new NodeI(cG,
                            this->getId(),
                            vIds,
                            this->getLayerId(),
                            this->getMatId());
    delete vIds;
    return res;
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

template class Node<Real>;
template class Node<Int >;
