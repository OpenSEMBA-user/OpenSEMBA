/*
 * Coordinate.cpp
 *
 *  Created on: Aug 26, 2012
 *      Author: luis
 */
#ifndef COORDINATE_H_
#include "Coordinate.h"
#endif
// =============== Coordinate =================================================
template<class T, int D>
Coordinate<T,D>::Coordinate() {
	id = 0;
}

template<class T, int D>
Coordinate<T,D>::Coordinate(unsigned int id_, const CartesianVector<T,D>& pos)
 : CartesianVector<T,D>(pos) {
	id = id_;
}

template<class T, int D>
Coordinate<T,D>::Coordinate(const CartesianVector<T,D>& pos)
 : CartesianVector<T,D>(pos) {
	id = 0;
}

template<class T, int D>
Coordinate<T,D>::~Coordinate() {

}

template<class T, int D>
Coordinate<T,D>&
Coordinate<T,D>::operator=(const Coordinate& rhs) {
	if (this == &rhs)
		return *this;
	id = rhs.id;
	for (unsigned int i = 0; i < D; i++) {
		this->val[i] = rhs.val[i];
	}
	return *this;
}

template<class T, int D>
inline bool
Coordinate<T,D>::operator==(const Coordinate& rhs) const {
	return id == rhs.id;
}

template<class T, int D>
CartesianVector<double,3>
Coordinate<T,D>::pos() const {
	CartesianVector<T,D> res;
	for (unsigned int i = 0; i < D; i++) {
		res.val[i] = this->val[i];
	}
	return res;
}

template<class T, int D>
inline unsigned int
Coordinate<T,D>::getId() const {
	return id;
}

template<class T, int D>
void
Coordinate<T,D>::printInfo() const {
	cout << "Id: ("<< id << ")  Pos: (";
	for (register int i = 0; i < D; i++) {
	   cout << this->val[i];
	   if (i < D-1) {
		   cout << " , ";
	   }
	}
	cout << ")";
}
