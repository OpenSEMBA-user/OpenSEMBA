#ifndef ELEMENT_H_
	#include "Element.h"
#endif

Element::Element() {
    id = 0;
    matId = 0;
    layerId = 0;
}

Element::Element(
 const uint idNew, const uint matIdNew, const uint layerIdNew) {
    id = idNew;
    matId = matIdNew;
    layerId = layerIdNew;
}

Element::~Element() {

}

void
Element::ascendingOrder(unsigned int nVal, unsigned int* val) const {
	unsigned int *res;
	res = new unsigned int [nVal];
	unsigned int maxV = 0;
	for (unsigned int i = 0; i < 3; i++) {
		val[i] > maxV ? maxV = val[i] : maxV;
	}
	for (unsigned int j = 0; j < 3; j++) {
		unsigned int iMin = 0;
		unsigned int currentMinV = maxV;
		for (unsigned int i = 0; i < 3; i++) {
			if (val[i] < currentMinV) {
				currentMinV = val[i];
				iMin = i;
			}
		}
		res[j] = val[iMin];
		val[iMin] = maxV;
	}
	for (unsigned int i = 0; i < nVal; i++) {
		val[i] = res[i];
	}
	delete[] res;
}

void
Element::setMatId(const unsigned int newMatId) {
	matId = newMatId;
}

void
Element::printInfo() const {
    cout<< "Element. Id: " << getId() << " MatId: " << getMatId()
        << " LayerId: " << getLayerId() << endl;
}

BoxD3
Element::getBound() const {
	BoxD3 res;
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		res << getV(i)->pos();
	}
	return res;
}

const CoordD3*
Element::getMinV() const {
	assert(getV(0) != NULL);
	const CoordD3*  res = getV(0);
	for (uint i = 1; i < numberOfVertices(); i++) {
		for (uint j = 0; j < 3; j++) {
			double val1 = getV(i)->pos()(j);
			double val2 = res->pos()(j);
			if(abs(val1 - val2) > tolerance) {
				if(val1 < val2) {
					res = getV(i);
				}
			}
		}
	}
	return res;
}

const CoordD3*
Element::getMaxV() const {
	assert(getV(0) != NULL);
	const CoordD3*  res = getV(0);
	for (uint i = 1; i < numberOfVertices(); i++) {
		for (uint j = 0; j < 3; j++) {
			double val1 = getV(i)->pos()(j);
			double val2 = res->pos()(j);
			if(abs(val1 - val2) > tolerance) {
				if(val1 > val2) {
					res = getV(i);
				}
			}
		}
	}
	return res;
}

void
Element::setV(
 const unsigned int i,
 const CoordD3* constCoordinate) {
	cout<< "ERROR @ setV for Element: "
	    << "Setting coordinates is not allowed for this element: "<<endl;
	printInfo();
}

bool
Element::isCoordinate(const CoordD3* coord) const {
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		if (*getV(i) == *coord) {
			return true;
		}
	}
	return false;
}

bool
Element::isInnerPoint(const CVecD3& pos) const {
    cerr << "ERROR @ Element: "
         << "Can't determine inner point for this element." << endl;
    printInfo();
    return false;
}

Element&
Element::operator =(const Element& rhs) {
    if (&rhs == this) {
        return *this;
    }
    id = rhs.id;
    matId = rhs.matId;
    layerId = rhs.layerId;
    return *this;
}
