#ifndef ELEMENT_H_
	#include "Element.h"
#endif

Element::Element() {
	matId = 0;
	id = 0;
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

pair<CVecD3, CVecD3>
Element::getInfinityBound() const {
	// Initializes bound.
	pair<CVecD3, CVecD3> res;
	for (unsigned int j = 0; j < 3; j++) {
		res.first(j) = -numeric_limits<double>::infinity();
		res.second(j) = +numeric_limits<double>::infinity();
	}
	return res;
}

pair<CVecD3,CVecD3>
Element::getBound() const {
	// Initializes bound.
	pair<CVecD3, CVecD3> res = getInfinityBound();
	// Shrinks bound.
	for (unsigned int i = 0; i < numberOfCoordinates(); i++) {
		Coordinate<double,3> coord = *getV(i);
		for (unsigned int j = 0; j < 3; j++) {
			if (coord(j) > res.first(j)) {
				res.first(j) = coord(j);
			}
			if (coord(j) < res.second(j)) {
				res.second(j) = coord(j);
			}
		}
	}
	for (unsigned int j = 0; j < 3; j++) {
		if (res.first(j) > res.second(j)) {
			swap(res.first(j), res.second(j));
		}
	}
	return res;
}

const Coordinate<double, 3>*
Element::getMinV() const {
	assert(getV(0) != NULL);
	const Coordinate<double, 3>*  res = getV(0);
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

const Coordinate<double, 3>*
Element::getMaxV() const {
	assert(getV(0) != NULL);
	const Coordinate<double, 3>*  res = getV(0);
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
 const Coordinate<double, 3>* constCoordinate) {
	cout<< "ERROR @ setV for Element: " << endl;
	printInfo();
	cout<< "Setting coordinates is not allowed for this element."<<endl;
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
