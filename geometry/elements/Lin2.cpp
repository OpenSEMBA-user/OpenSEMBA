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
bool Lin2<T>::isStructured(const Grid3& grid, const Real tol) const {
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        if (!grid.isCell(*this->getV(i)), tol) {
            return false;
        }
    }
    Box<T,3> bound(*this->getMinV(), *this->getMaxV());
    if (!bound.isLine()) {
        return false;
    }
    vector< CartesianVector<T,3> > pos = bound.getPos();
    vector<bool> found(pos.size(), false);
    if (pos.size() != this->numberOfCoordinates()) {
        return false;
    }
    bool foundCoord;
    for(UInt i = 0; i < this->numberOfCoordinates(); i++) {
        foundCoord = false;
        for(UInt j= 0; j < pos.size(); j++) {
            if (found[j]) {
                continue;
            }
            if (*this->getV(i) == pos[j]) {
                foundCoord = true;
                found[j] = true;
                break;
            }
        }
        if (!foundCoord) {
            return false;
        }
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
    if (this->template is<ElemI>()) {
        return NULL;
    }
    if (!isStructured(grid, tol)) {
        return NULL;
    }
    CVecI3 cell;
    const CoordI3* coord;
    CoordinateId coordId;
    CoordinateId* vIds = new CoordinateId[this->numberOfCoordinates()];
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        cell  = grid.getCell(*this->getV(i));
        coordId = this->getV(i)->getId();
        if (!cG.existId(coordId)) {
            cG.add(new CoordI3(coordId, cell));
        }
        coord = cG.getPtrToId(coordId);
        if (*coord != cell) {
            cerr << endl << "ERROR @ Element::toStructured(): "
                 << "Existent Coordinate not coincident." << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        vIds[i] = coordId;
    }
    ElemI* res =  new LinI2(cG,
                            this->getId(),
                            vIds,
                            this->getLayerId(),
                            this->getMatId());
    delete vIds;
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

template class Lin2<Real>;
template class Lin2<Int >;
