/*
 * Coordinate.h
 *
 *  Created on: Aug 26, 2012
 *      Author: luis
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <vector>
#include <stdlib.h>
#include "../math/CartesianVector.h"

using namespace std;

#ifndef COORDINATE_ERROR
#define COORDINATE_ERROR 13
#endif
// ====================================================================
template <class T, int D>
class Coordinate : public CartesianVector<T,D> {
public:
	unsigned int id;
	Coordinate();
	Coordinate(unsigned int id_, const CartesianVector<T,D>& pos);
	Coordinate(const CartesianVector<T,D>& pos);
	virtual ~Coordinate();
	Coordinate&
	 operator=(const Coordinate& rhs);
	bool
	 operator==(const Coordinate& rhs) const;
	CartesianVector<double,3>
	 pos() const;
	void
	 printInfo() const;
	unsigned int
	 getId() const;
};

#include "Coordinate.cpp"

typedef Coordinate<double, 3> CoordD3;

#endif /* COORDINATE_H_ */
