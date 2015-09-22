#include "Element.h"

ElementBase::ElementBase(const LayerId layId,
                         const MatId   matId) {
    layId_ = layId;
    matId_ = matId;
}

ElementBase::ElementBase(const ElementBase& rhs) {
    layId_ = rhs.layId_;
    matId_ = rhs.matId_;
}

ElementBase::~ElementBase() {

}

bool ElementBase::operator==(const ElementBase& rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        return true;
    }
    return false;
}

bool ElementBase::operator!=(const ElementBase& rhs) const {
    return !(*this == rhs);
}

vector<CoordinateId> ElementBase::ascendingIdOrder(
        const vector<CoordinateId>& in) {
    vector<CoordinateId> res = in;
    sort(res.begin(), res.end());
    return res;
}

void ElementBase::printInfo() const {
    cout << "Element. Id: " << this->getId()
         << " MatId: " << this->getMatId()
         << " LayerId: " << this->getLayerId() << endl;
}

template<class T>
Element<T>::ErrorCoord::ErrorCoord(const CoordinateId& coordId) {
    coordId_ = coordId;
}

template<class T>
Element<T>::ErrorCoord::~ErrorCoord() throw () {

}

template<class T>
Element<T>::ErrorCoordNotFound::ErrorCoordNotFound(
        const CoordinateId& coordId)
:   ErrorCoord(coordId) {
    stringstream aux;
    aux << "Coordinate with Id (" << coordId << ") not found";
    this->setMsg(aux.str());
}

template<class T>
Element<T>::ErrorCoordNotFound::~ErrorCoordNotFound() throw () {

}

template<class T>
Element<T>::ErrorCoordNotCoincident::ErrorCoordNotCoincident(
        const CoordinateId& coordId)
:   ErrorCoord(coordId) {
    stringstream aux;
    aux << "Coordinate with Id (" << coordId
        << ") not coincident with previous Coordinate";
    this->setMsg(aux.str());
}

template<class T>
Element<T>::ErrorCoordNotCoincident::~ErrorCoordNotCoincident() throw () {
}

template<class T>
Element<T>::Element() {

}

template<class T>
Element<T>::~Element() {

}

template<class T>
bool Element<T>::operator==(const ElementBase& rhs) const {
    if (!ElementBase::operator==(rhs)) {
        return false;
    }
    const Element<T>* rhsPtr = rhs.castTo<Element<T>>();
    bool res = true;
    res &= (this->getId() == rhsPtr->getId());
    for (UInt i = 0; i < this->numberOfCoordinates(); i++) {
        res &= (*this->getV(i) == *rhsPtr->getV(i));
    }
    res &= (this->getLayerId() == rhsPtr->getLayerId());
    res &= (this->getMatId() == rhsPtr->getMatId());

    return res;
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
    throw ErrorNotImplemented("Element::isInnerPoint");
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
            if(MathUtils::greater(val1, val2, res->pos().norm())) {
                res = getVertex(i);
                break;
            }
        }
    }
    return res;
}

template<class T>
vector<const Coordinate<T,3>*> Element<T>::getVertices() const {
    vector<const Coordinate<T,3>*> res(numberOfVertices());
    for (UInt i = 0; i < numberOfVertices(); i++) {
        res[i] = getVertex(i);
    }
    return res;
}

template<class T>
vector<const Coordinate<T,3>*> Element<T>::getSideVertices(
        const UInt face) const {
    vector<const Coordinate<T,3>*> res(numberOfSideVertices());
    for (UInt i = 0; i < numberOfSideVertices(); i++) {
        res[i] = getSideVertex(face,i);
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
ElemI* Element<T>::toStructured(const GroupCoordinates<CoordI3>& cG,
                                const Grid3& grid, const Real tol) const {
    return NULL;
}

template<class T>
ElemR* Element<T>::toUnstructured(const GroupCoordinates<CoordR3>& cG,
                                  const Grid3& grid) const {
    return NULL;
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
CoordinateId* Element<T>::vertexToStructured(
        const GroupCoordinates<CoordI3>& cG,
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
            delete [] vIds;
            throw ErrorCoordNotFound(coordId);
        }
        coord = cG.getId(coordId);
        if (coord->pos() != cell) {
            delete [] vIds;
            throw ErrorCoordNotCoincident(coordId);
        }
        vIds[i] = coordId;
    }
    return vIds;
}

template<class T>
CoordinateId* Element<T>::vertexToUnstructured(
        const GroupCoordinates<CoordR3>& cG,
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
            delete [] vIds;
            throw ErrorCoordNotFound(coordId);
        }
        coord = cG.getId(coordId);
        if (coord->pos() != pos) {
            delete [] vIds;
            throw ErrorCoordNotCoincident(coordId);
        }
        vIds[i] = coordId;
    }
    return vIds;
}

template class Element<Real>;
template class Element<Int >;
