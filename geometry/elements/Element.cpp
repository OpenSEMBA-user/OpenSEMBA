#include "Element.h"

const Real Element::tolerance = 1e-15;

Element::Element(const LayerId layerId,
                 const MatId   matId)
:   ElementBase(layerId, matId) {

}

Element::Element(const ElementId id,
                 const LayerId layerId,
                 const MatId   matId)
:   ElementBase(id, layerId, matId) {

}

Element::Element(const Element& rhs)
:   ElementBase(rhs) {

}

Element::~Element() {

}

bool Element::isCoordinate(const CoordR3* coord) const {
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        if (*getV(i) == *coord) {
            return true;
        }
    }
    return false;
}

bool Element::isInnerPoint(const CVecR3& pos) const {
    cerr << "ERROR @ Element: "
         << "Can't determine inner point for this element." << endl;
    printInfo();
    return false;
}

BoxR3 Element::getBound() const {
    BoxR3 res;
    for (UInt i = 0; i < numberOfCoordinates(); i++) {
        res << getV(i)->pos();
    }
    return res;
}

const CoordR3* Element::getMinV() const {
    assert(getV(0) != NULL);
    const CoordR3*  res = getV(0);
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

const CoordR3* Element::getMaxV() const {
    assert(getV(0) != NULL);
    const CoordR3*  res = getV(0);
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

void Element::setV(const UInt i, const CoordR3* coord) {
    cout << "ERROR @ setV for Element: "
         << "Setting coordinates is not allowed for this element: "
         << endl;
    
	printInfo();
}

void Element::printInfo() const {
    cout<< "Element. Id: " << getId() << " MatId: " << getMatId()
        << " LayerId: " << getLayerId() << endl;
}

void Element::ascendingOrder(UInt nVal, UInt* val) const {
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
