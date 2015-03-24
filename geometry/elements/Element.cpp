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
    const Coordinate<T,3>* res = getV(0);
    for (UInt i = 1; i < numberOfVertices(); i++) {
        if(res->pos() == getV(0)->pos()) {
            continue;
        }
        for (UInt j = 0; j < 3; j++) {
            Real val1 = getV(i)->pos()(j);
            Real val2 = res->pos()(j);
            if(MathUtils::lower(val1, val2, res->pos().norm())) {
                res = getV(i);
            }
        }
    }
    return res;
}

template<class T>
const Coordinate<T,3>* Element<T>::getMaxV() const {
    assert(getV(0) != NULL);
    const Coordinate<T,3>* res = getV(0);
    for (UInt i = 1; i < numberOfVertices(); i++) {
        if(res->pos() == getV(0)->pos()) {
            continue;
        }
        for (UInt j = 0; j < 3; j++) {
            Real val1 = getV(i)->pos()(j);
            Real val2 = res->pos()(j);
            if(MathUtils::greather(val1, val2, res->pos().norm())) {
                res = getV(i);
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
void Element<T>::printInfo() const {
    cout<< "Element. Id: " << this->getId() << " MatId: " << this->getMatId()
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

template class Element<Real>;
template class Element<Int >;
