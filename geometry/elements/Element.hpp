#include "Element.h"

template<class T>
const Real Element<T>::tolerance = 1e-15;

template<class T>
Element<T>::Element(const LayerId layerId,
                    const MatId   matId)
:   ElementBase(layerId, matId) {

}

template<class T>
Element<T>::Element(const ElementId id,
                    const LayerId layerId,
                    const MatId   matId)
:   ElementBase(id, layerId, matId) {

}

template<class T>
Element<T>::Element(const Element<T>& rhs)
:   ElementBase(rhs) {

}

template<class T>
Element<T>::~Element() {

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
        for (UInt j = 0; j < 3; j++) {
            Real val1 = getV(i)->pos()(j);
            Real val2 = res->pos()(j);
            if(abs(val1 - val2) > tolerance) {
                if(val1 < val2) {
                    res = getV(i);
                }
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
        for (UInt j = 0; j < 3; j++) {
            Real val1 = getV(i)->pos()(j);
            Real val2 = res->pos()(j);
            if(abs(val1 - val2) > tolerance) {
                if(val1 > val2) {
                    res = getV(i);
                }
            }
        }
    }
    return res;
}

template<class T>
void Element<T>::setV(const UInt i, const Coordinate<T,3>* coord) {
    cout << "ERROR @ setV for Element: "
         << "Setting coordinates is not allowed for this element: "
         << endl;
    
	printInfo();
}

template<class T>
void Element<T>::printInfo() const {
    cout<< "Element. Id: " << getId() << " MatId: " << getMatId()
        << " LayerId: " << getLayerId() << endl;
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
