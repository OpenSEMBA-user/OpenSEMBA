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
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = coordGr.getPtrToId(vId[i]);
	}
}

template<class T>
Node<T>::Node(const ElementId id,
              const Coordinate<T,3>* v[1],
              const LayerId layerId,
              const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

    v_[0] = v[0];
}
template<class T>
Node<T>::Node(CoordinateGroup<Coordinate<T,3> >& cG,
              const ElementId id,
              const Box<T,3>& box,
              const LayerId layerId,
              const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

    if(!box.isPoint()) {
        cerr << endl << "ERROR @ Node::Node(): "
                     << "Box is not a Point" << endl;
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
Node<T>::Node(const Node<T>& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {

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
bool Node<T>::isStructured(const Grid3& grid, const Real tol) const {
    if (!this->vertexInCell(grid,tol)) {
        return false;
    }
    if (!this->getBound().isPoint()) {
        return false;
    }
    if (!this->vertexInBound()) {
        return false;
    }
    return true;
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
                             const Grid3& grid, const Real tol) const {
    CoordinateId* vIds = this->vertexToStructured(cG, grid, tol);
    if (vIds == NULL) {
        return NULL;
    }
    ElemI* res =  new NodeI(cG,
                            this->getId(),
                            vIds,
                            this->getLayerId(),
                            this->getMatId());
    delete[] vIds;
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
