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

#include "CoordinateBase.h"

#ifndef COORDINATE_ERROR
#define COORDINATE_ERROR 13
#endif
// ====================================================================
template <class T, int D>
class Coordinate : public CoordinateBase,
                   public CartesianVector<T,D> {
public:
    Coordinate();
    Coordinate(const CoordinateId id_, const CartesianVector<T,D>& pos);
    Coordinate(const CartesianVector<T,D>& pos);
    virtual ~Coordinate();

    virtual ClassBase* clone()                      const;
    virtual ClassBase* clone(const CoordinateId id) const;

    Coordinate& operator=(const Coordinate& rhs);

    bool operator==(const Coordinate& rhs) const;

    CartesianVector<T,3> pos() const;

    void printInfo() const;
};

#include "Coordinate.hpp"

typedef Coordinate<double  , 3> CoordD3;
typedef Coordinate<long int, 3> CoordI3;

#endif /* COORDINATE_H_ */
