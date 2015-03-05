#include "Element.h"

const double Element::tolerance = 1e-15;

Element::Element(const uint layerId,
                 const uint matId)
:   ElementBase(layerId, matId) {

}

Element::Element(const ElementId id,
                 const uint layerId,
                 const uint matId)
:   ElementBase(id, layerId, matId) {

}

Element::Element(const Element& rhs)
:   ElementBase(rhs) {

}

Element::Element(const ElementId id, const Element& rhs)
:   ElementBase(id, rhs) {

}

Element::~Element() {

}

bool Element::isCoordinate(const CoordD3* coord) const {
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		if (*getV(i) == *coord) {
			return true;
		}
	}
	return false;
}

bool Element::isInnerPoint(const CVecD3& pos) const {
    cerr << "ERROR @ Element: "
         << "Can't determine inner point for this element." << endl;
    printInfo();
    return false;
}

BoxD3 Element::getBound() const {
	BoxD3 res;
	for (uint i = 0; i < numberOfCoordinates(); i++) {
		res << getV(i)->pos();
	}
	return res;
}

const CoordD3* Element::getMinV() const {
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

const CoordD3* Element::getMaxV() const {
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

void Element::setV(const uint i, const CoordD3* coord) {
	cout << "ERROR @ setV for Element: "
	     << "Setting coordinates is not allowed for this element: "
         << endl;
    
	printInfo();
}

void Element::printInfo() const {
    cout<< "Element. Id: " << getId() << " MatId: " << getMatId()
        << " LayerId: " << getLayerId() << endl;
}

void Element::ascendingOrder(uint nVal, uint* val) const {
	uint *res;
	res = new uint [nVal];
	uint maxV = 0;
	for (uint i = 0; i < 3; i++) {
		val[i] > maxV ? maxV = val[i] : maxV;
	}
	for (uint j = 0; j < 3; j++) {
		uint iMin = 0;
		uint currentMinV = maxV;
		for (uint i = 0; i < 3; i++) {
			if (val[i] < currentMinV) {
				currentMinV = val[i];
				iMin = i;
			}
		}
		res[j] = val[iMin];
		val[iMin] = maxV;
	}
	for (uint i = 0; i < nVal; i++) {
		val[i] = res[i];
	}
	delete[] res;
}
