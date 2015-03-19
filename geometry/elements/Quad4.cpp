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
Quad4<T>::Quad4(const CoordinateGroup<Coordinate<T,3> >& cG,
                const ElementId id,
                const CoordinateId vId[4],
                const LayerId layerId,
                const MatId   matId)
:   Elem(id, layerId, matId) {
    
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = cG.getPtrToId(vId[i]);
	}
	// TODO Normals are not handled.
	check();
}

template<class T>
Quad4<T>::Quad4(CoordinateGroup<Coordinate<T,3> >& cG,
                const ElementId id,
                const Box<T,3>& box,
                const LayerId layerId,
                const MatId   matId)
:   Elem(id, layerId, matId) {

    if(!box.isSurface()) {
        cerr << endl << "ERROR @ Quad4::Quad4(): "
                     << "Box is not a Surface" << endl;
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
Quad4<T>::Quad4(const Quad4<T>& rhs)
:   Elem(rhs) {
    
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
ElemI* Quad4<T>::toStructured(CoordinateGroup<CoordI3>& cG,
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
    Element<Int>* res =  new QuadI4(cG,
                                    this->getId(),
                                    vIds,
                                    this->getLayerId(),
                                    this->getMatId());
    delete vIds;
    return res;
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

template class Quad4<Real>;
template class Quad4<Int >;
