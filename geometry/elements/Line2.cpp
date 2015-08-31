/*
 * Line2.cpp
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#include <geometry/elements/Line2.h>

template<class T>
const SimplexLin<1> Line2<T>::lin;

template<class T>
Line2<T>::Line2() {

}

template<class T>
Line2<T>::Line2(const GroupCoordinates<Coordinate<T,3> >& cG,
              const ElementId id,
              const CoordinateId vId[2],
              const LayerId layerId,
              const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {
    
    setCoordinates(cG, vId);
}

template<class T>
Line2<T>::Line2(const ElementId id,
              const Coordinate<T,3>* v[2],
              const LayerId layerId,
              const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {
    
    setCoordinates(v);
}

template<class T>
Line2<T>::Line2(GroupCoordinates<Coordinate<T,3> >& cG,
              const ElementId id,
              const Box<T,3>& box,
              const LayerId layerId,
              const MatId   matId)
:   ClassIdBase<ElementId>(id),
    Elem(layerId, matId) {

    setCoordinates(cG, box);
}


template<class T>
Line2<T>::Line2(const GroupCoordinates<Coordinate<T,3> >& cG,
              const CoordinateId vId[2]) {
    setCoordinates(cG, vId);
}

template<class T>
Line2<T>::Line2(const Coordinate<T,3>* v[2]) {
    setCoordinates(v);
}

template<class T>
Line2<T>::Line2(GroupCoordinates<Coordinate<T,3> >& cG,
              const Box<T,3>& box) {

    setCoordinates(cG, box);
}


template<class T>
Line2<T>::Line2(const Line2<T>& rhs)
:   ClassIdBase<ElementId>(rhs),
    Elem(rhs) {
    
    for (UInt i = 0; i < lin.np; i++) {
		v_[i] = rhs.v_[i];
	}
}

template<class T>
Line2<T>::~Line2() {

}

template<class T>
bool Line2<T>::isStructured(const Grid3& grid, const Real tol) const {
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
const Coordinate<T,3>* Line2<T>::getSideV(const UInt f, const UInt i) const {
    return v_[i];
}

template<class T>
const Coordinate<T,3>* Line2<T>::getVertex(const UInt i) const {
	return v_[i];
}

template<class T>
const Coordinate<T,3>* Line2<T>::getSideVertex(const UInt f,
                                              const UInt i) const {
    return v_[i];
}

template<class T>
void Line2<T>::setV(const UInt i, const Coordinate<T,3>* coord) {

    assert(i < numberOfCoordinates());
    v_[i] = coord;
}

template<class T>
ElemI* Line2<T>::toStructured(const GroupCoordinates<CoordI3>& cG,
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
ElemR* Line2<T>::toUnstructured(const GroupCoordinates<CoordR3>& cG,
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
void Line2<T>::printInfo() const {
    cout << "--- Lin2 info ---" << endl;
    Line<T>::printInfo();
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i]->printInfo();
    }
}

template<class T>
void Line2<T>::setCoordinates(const GroupCoordinates<Coordinate<T,3> >& cG,
                              const CoordinateId vId[2]) {
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = cG.getId(vId[i]);
    }
}

template<class T>
void Line2<T>::setCoordinates(const Coordinate<T,3>* v[2]) {
    for (UInt i = 0; i < lin.np; i++) {
        v_[i] = v[i];
    }
}

template<class T>
void Line2<T>::setCoordinates(GroupCoordinates<Coordinate<T,3> >& cG,
                             const Box<T,3>& box) {
    if(!box.isLine()) {
        throw typename Box<T,3>::ErrorNotLine();
    }
    vector<CartesianVector<T,3> > pos = box.getPos();
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        v_[i] = cG.getPos(pos[i]);
        if (v_[i] == NULL) {
            v_[i] = cG.addPos(pos[i]);
        }
    }
}

template class Line2<Real>;
template class Line2<Int >;
