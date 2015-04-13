/*
 * Coordinate.h
 *
 *  Created on: Aug 26, 2012
 *      Author: luis
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <cstdlib>
#include <sstream>
#include <type_traits>
#include <vector>

using namespace std;

#include "../math/CartesianVector.h"

#include "../../base/class/ClassIdBase.h"
#include "../../base/class/ClassCompBase.h"

CREATE_ID(CoordinateId);

class CoordinateBase : public virtual ClassIdBase<CoordinateId>,
                       public virtual ClassCompBase {
public:
    CoordinateBase() {}
    virtual ~CoordinateBase() {}
};

template <class T, Int D>
class Coordinate : public virtual CoordinateBase,
                   public virtual CartesianVector<T,D> {
public:
    Coordinate();
    Coordinate(const CoordinateId id_, const CartesianVector<T,D>& pos);
    explicit Coordinate(const CartesianVector<T,D>& pos);
    Coordinate(const Coordinate& rhs);
    virtual ~Coordinate();

    DEFINE_CLONE((Coordinate<T,D>));

    Coordinate& operator=(const Coordinate& rhs);

    bool operator==(const ClassCompBase& rhs) const;
    bool operator!=(const ClassCompBase& rhs) const;
    bool operator< (const ClassCompBase& rhs) const;

    CartesianVector<T,D>& pos();
    const CartesianVector<T,D>& pos() const;

    void printInfo() const;
};

typedef CoordinateBase      Coord;
typedef Coordinate<Real, 3> CoordR3;
typedef Coordinate<Int , 3> CoordI3;

#endif /* COORDINATE_H_ */
