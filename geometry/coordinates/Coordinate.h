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

using namespace std;

#include "../math/CartesianVector.h"

#include "../../base/ClassIdBase.h"

CreateId(CoordinateId);

template <class T = void, Int D = 0>
class Coordinate;

template <>
class Coordinate<void,0> : public virtual ClassIdBase<CoordinateId> {
public:
    Coordinate() {}
    virtual ~Coordinate() {}
};

template <class T, Int D>
class Coordinate : public virtual Coordinate<void,0>,
                   public virtual CartesianVector<T,D> {
public:
    Coordinate();
    Coordinate(const CoordinateId id_, const CartesianVector<T,D>& pos);
    Coordinate(const CartesianVector<T,D>& pos);
    Coordinate(const Coordinate& rhs);
    virtual ~Coordinate();

    ClassBase* clone() const;

    Coordinate& operator=(const Coordinate& rhs);

    bool operator==(const Coordinate& rhs) const;
    bool operator!=(const Coordinate& rhs) const;

    CartesianVector<T,D> pos() const;

    void printInfo() const;
};

typedef Coordinate<void, 0> Coord;
typedef Coordinate<Real, 3> CoordR3;
typedef Coordinate<Int , 3> CoordI3;

#endif /* COORDINATE_H_ */
