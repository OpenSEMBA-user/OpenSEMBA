#include "Element.h"

Element<void>::Element(const LayerId layId,
                       const MatId   matId)
:   layerId_(layId),
    matId_  (matId  ) {

}

Element<void>::Element(const Element& rhs)
:   layerId_(rhs.layerId_),
    matId_  (rhs.matId_  ) {

}

Element<void>::~Element() {

}

template<class T>
Element<T>::Element() {

}

template<class T>
Element<T>::~Element() {

}

template<class T>
bool Element<T>::operator==(const Element<T>& rhs) const {
    bool res = true;
    res &= this->getLayerId() == rhs.getLayerId();
    res &= this->getMatId() == rhs.getMatId();
    res &= this->isCurved() == rhs.isCurved();
    res &= this->isQuadratic() == rhs.isQuadratic();
    res &= this->numberOfFaces() == rhs.numberOfFaces();
    res &= this->numberOfVertices() == rhs.numberOfVertices();
    res &= this->numberOfCoordinates() == rhs.numberOfCoordinates();
    if (!res) {
        return false;
    }
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        res &= (*this->getV(i) == *rhs.getV(i));
    }
    for (UInt i = 0; i < this->numberOfVertices(); i++) {
        res &= (*this->getVertex(i) == *rhs.getVertex(i));
    }
    for (UInt f = 0; f < this->numberOfFaces(); f++) {
        res &= this->numberOfSideCoordinates(f) ==
                   rhs.numberOfSideCoordinates(f);
        res &= this->numberOfSideVertices(f) ==
                   rhs.numberOfSideVertices(f);
        if (!res) {
            return false;
        }
        for (UInt i = 0; i < this->numberOfSideCoordinates(f); i++) {
            res &= (*this->getSideV(f,i) == *rhs.getSideV(f,i));
        }
        for (UInt i = 0; i < this->numberOfSideVertices(f); i++) {
            res &= (*this->getSideVertex(f,i) == *rhs.getSideVertex(f,i));
        }
    }
    return res;
}

template<class T>
bool Element<T>::operator!=(const Element<T>& rhs) const {
    return !(*this == rhs);
}

template<class T>
bool Element<T>::isCoordinate(const Coordinate<T,3>* coord) const {
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        if (*getV(i) == *coord) {
            return true;
        }
    }
    return false;
}

template<class T>
bool Element<T>::isStructured(const Grid3& grid, const Real tol) const {
    return false;
}

template<class T>
bool Element<T>::isInnerPoint(const CartesianVector<T,3>& pos) const {
    cerr << "ERROR @ Element: "
         << "Can't determine inner point for this element." << endl;
    printInfo();
    return false;
}

template<class T>
Box<T,3> Element<T>::getBound() const {
    Box<T,3> res;
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        res << getV(i)->pos();
    }
    return res;
}

template<class T>
const Coordinate<T,3>* Element<T>::getMinV() const {
    assert(getV(0) != NULL);
    const Coordinate<T,3>* res = getVertex(0);
    for (UInt i = 1; i < numberOfVertices(); i++) {
        if(res->pos() == getVertex(i)->pos()) {
            continue;
        }
        for (UInt j = 0; j < 3; j++) {
            Real val1 = getVertex(i)->pos()(j);
            Real val2 = res->pos()(j);
            if(MathUtils::lower(val1, val2, res->pos().norm())) {
                res = getVertex(i);
                break;
            }
        }
    }
    return res;
}

template<class T>
const Coordinate<T,3>* Element<T>::getMaxV() const {
    assert(getV(0) != NULL);
    const Coordinate<T,3>* res = getVertex(0);
    for (UInt i = 1; i < numberOfVertices(); i++) {
        if(res->pos() == getVertex(i)->pos()) {
            continue;
        }
        for (UInt j = 0; j < 3; j++) {
            Real val1 = getVertex(i)->pos()(j);
            Real val2 = res->pos()(j);
            if(MathUtils::greather(val1, val2, res->pos().norm())) {
                res = getVertex(i);
                break;
            }
        }
    }
    return res;
}

template<class T>
void Element<T>::setV(const UInt i, const Coordinate<T,3>* coord) {
    cout << "ERROR @ Element::setV(): "
         << "Setting coordinates is not allowed for this element"
         << endl;
    
	printInfo();
}

template<class T>
ElemI* Element<T>::toStructured(CoordinateGroup<CoordI3>& cG,
                                const Grid3& grid, const Real tol) const {
    return NULL;
}

template<class T>
ElemR* Element<T>::toUnstructured(CoordinateGroup<CoordR3>& cG,
                                  const Grid3& grid) const {
    return NULL;
}

template<class T>
void Element<T>::printInfo() const {
    cout << "Element. Id: " << this->getId()
                 << " MatId: " << this->getMatId()
                 << " LayerId: " << this->getLayerId() << endl;
}

template<class T>
void Element<T>::ascendingOrder(UInt nVal, UInt* val) const {
    UInt *res;
    res = new UInt [nVal];
    UInt maxV = 0;
    for (UInt i = 0; i < 3; i++) {
        val[i] > maxV ? maxV = val[i] : maxV;
    }
    for (UInt j = 0; j < 3; j++) {
        UInt iMin = 0;
        UInt currentMinV = maxV;
        for (UInt i = 0; i < 3; i++) {
            if (val[i] < currentMinV) {
                currentMinV = val[i];
                iMin = i;
            }
        }
        res[j] = val[iMin];
        val[iMin] = maxV;
    }
    for (UInt i = 0; i < nVal; i++) {
        val[i] = res[i];
    }
    delete[] res;
}

template<class T>
bool Element<T>::vertexInCell(const Grid3& grid, const Real tol) const {
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        if (!grid.isCell(*this->getV(i), tol)) {
            return false;
        }
    }
    return true;
}

template<class T>
bool Element<T>::vertexInBound() const {
    Box<T,3> bound = this->getBound();
    vector< CartesianVector<T,3> > pos = bound.getPos();
    if (pos.size() != this->numberOfCoordinates()) {
        return false;
    }
    vector<bool> found(pos.size(), false);
    bool foundCoord;
    for(UInt i = 0; i < this->numberOfCoordinates(); i++) {
        foundCoord = false;
        for(UInt j= 0; j < pos.size(); j++) {
            if (found[j]) {
                continue;
            }
            if (this->getV(i)->pos() == pos[j]) {
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
CoordinateId* Element<T>::vertexToStructured(CoordinateGroup<CoordI3>& cG,
                                             const Grid3& grid,
                                             const Real tol) const {
    if (!this->is<ElemR>()) {
        return NULL;
    }
    if (!this->isStructured(grid, tol)) {
        return NULL;
    }
    CVecI3 cell;
    const CoordI3* coord;
    CoordinateId coordId;
    CoordinateId* vIds = new CoordinateId[this->numberOfCoordinates()];
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        cell  = grid.getCell(*this->getV(i), true, tol);
        coordId = this->getV(i)->getId();
        if (!cG.existId(coordId)) {
            cG.add(new CoordI3(coordId, cell));
        }
        coord = cG.getPtrToId(coordId);
        if (coord->pos() != cell) {
            cerr << endl << "ERROR @ Element::vertexToStructured(): "
                 << "Existent Coordinate not coincident." << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        vIds[i] = coordId;
    }
    return vIds;
}

template<class T>
CoordinateId* Element<T>::vertexToUnstructured(CoordinateGroup<CoordR3>& cG,
                                               const Grid3& grid) const {
    if (!this->is<ElemI>()) {
        return NULL;
    }
    CVecR3 pos;
    const CoordR3* coord;
    CoordinateId coordId;
    CoordinateId* vIds = new CoordinateId[this->numberOfCoordinates()];
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        pos = grid.getPos(*this->getV(i));
        coordId = this->getV(i)->getId();
        if (!cG.existId(coordId)) {
            cG.add(new CoordR3(coordId, pos));
        }
        coord = cG.getPtrToId(coordId);
        if (coord->pos() != pos) {
            cerr << endl << "ERROR @ Element::vertexToUnstructured(): "
                 << "Existent Coordinate not coincident." << endl;
            assert(false);
            exit(EXIT_FAILURE);
        }
        vIds[i] = coordId;
    }
    return vIds;
}

template class Element<Real>;
template class Element<Int >;
