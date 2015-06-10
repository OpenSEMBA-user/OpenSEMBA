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
Quad4<T>::Quad4(const GroupCoordinates<Coordinate<T,3> >& cG,
                const ElementId id,
                const CoordinateId vId[4],
                const LayerId layerId,
                const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {
    
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = cG.get(vId[i]);
	}
	// TODO Normals are not handled.
	check();
}

template<class T>
Quad4<T>::Quad4(GroupCoordinates<Coordinate<T,3> >& cG,
                const ElementId id,
                const Box<T,3>& box,
                const LayerId layerId,
                const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

    if(!box.isSurface()) {
        throw typename Box<T,3>::ErrorNotSurface();
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
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {
    
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = rhs.v_[i];
    }
}

template<class T>
Quad4<T>::~Quad4() {
    
}

template<class T>
bool Quad4<T>::isStructured(const Grid3& grid, const Real tol) const {
    if (!this->vertexInCell(grid,tol)) {
        return false;
    }
    if (!this->getBound().isSurface()) {
        return false;
    }
    if (!this->vertexInBound()) {
        return false;
    }
    return true;
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
ElemI* Quad4<T>::toStructured(const GroupCoordinates<CoordI3>& cG,
                              const Grid3& grid, const Real tol) const {
    CoordinateId* vIds = this->vertexToStructured(cG, grid, tol);
    if (vIds == NULL) {
        return NULL;
    }
    Element<Int>* res =  new QuaI4(cG,
                                   this->getId(),
                                   vIds,
                                   this->getLayerId(),
                                   this->getMatId());
    delete[] vIds;
    return res;
}

template<class T>
ElemR* Quad4<T>::toUnstructured(const GroupCoordinates<CoordR3>& cG,
                                const Grid3& grid) const {
    CoordinateId* vIds = this->vertexToUnstructured(cG, grid);
    if (vIds == NULL) {
        return NULL;
    }
    ElemR* res =  new QuaR4(cG,
                            this->getId(),
                            vIds,
                            this->getLayerId(),
                            this->getMatId());
    delete[] vIds;
    return res;
}

template<class T>
void Quad4<T>::printInfo() const {
	cout << "--- Quad4 info ---" << endl;
	Quad<T>::printInfo();
	for (UInt i = 0; i < numberOfCoordinates(); i++) {
	    v_[i]->printInfo();
	}
}

template<class T>
void Quad4<T>::check() const {
	// TODO Auto-generated
}

template class Quad4<Real>;
template class Quad4<Int >;
