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
Lin2<T>::Lin2(const CoordinateGroup<Coordinate<T,3> >& cG,
              const ElementId id,
              const CoordinateId vId[2],
              const LayerId layerId,
              const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {
    
    setCoordinates(cG, vId);
}

template<class T>
Lin2<T>::Lin2(const ElementId id,
              const Coordinate<T,3>* v[2],
              const LayerId layerId,
              const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {
    
    setCoordinates(v);
}

template<class T>
Lin2<T>::Lin2(CoordinateGroup<Coordinate<T,3> >& cG,
              const ElementId id,
              const Box<T,3>& box,
              const LayerId layerId,
              const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

    setCoordinates(cG, box);
}


template<class T>
Lin2<T>::Lin2(const CoordinateGroup<Coordinate<T,3> >& cG,
              const CoordinateId vId[2]) {
    setCoordinates(cG, vId);
}

template<class T>
Lin2<T>::Lin2(const Coordinate<T,3>* v[2]) {
    setCoordinates(v);
}

template<class T>
Lin2<T>::Lin2(CoordinateGroup<Coordinate<T,3> >& cG,
              const Box<T,3>& box) {

    setCoordinates(cG, box);
}


template<class T>
Lin2<T>::Lin2(const Lin2<T>& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {
    
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
bool Lin2<T>::isStructured(const Grid3& grid, const Real tol) const {
    if (!this->vertexInCell(grid,tol)) {
        return false;
    }
    if (!this->getBound().isLine()) {
        return false;
    }
    if (!this->vertexInBound()) {
        return false;
    }
    return true;
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
ElemI* Lin2<T>::toStructured(CoordinateGroup<CoordI3>& cG,
                             const Grid3& grid, const Real tol) const {
    CoordinateId* vIds = this->vertexToStructured(cG, grid, tol);
    if (vIds == NULL) {
        return NULL;
    }
    ElemI* res =  new LinI2(cG,
                            this->getId(),
                            vIds,
                            this->getLayerId(),
                            this->getMatId());
    delete[] vIds;
    return res;
}

template<class T>
ElemR* Lin2<T>::toUnstructured(CoordinateGroup<CoordR3>& cG,
                               const Grid3& grid) const {
    CoordinateId* vIds = this->vertexToUnstructured(cG, grid);
    if (vIds == NULL) {
        return NULL;
    }
    ElemR* res =  new LinR2(cG,
                            this->getId(),
                            vIds,
                            this->getLayerId(),
                            this->getMatId());
    delete[] vIds;
    return res;
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

template<class T>
void Lin2<T>::setCoordinates(const CoordinateGroup<Coordinate<T,3> >& cG,
                             const CoordinateId vId[2]) {
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = cG.getPtrToId(vId[i]);
    }
}

template<class T>
void Lin2<T>::setCoordinates(const Coordinate<T,3>* v[2]) {
    for (UInt i = 0; i < lin.np; i++) {
        v_[i] = v[i];
    }
}

template<class T>
void Lin2<T>::setCoordinates(CoordinateGroup<Coordinate<T,3> >& cG,
                             const Box<T,3>& box) {
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

template class Lin2<Real>;
template class Lin2<Int >;
