/*
 * Quad4.h
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#ifndef QUAD_H_
#define QUAD_H_

#include "Surface.h"

template<class T = void>
class Quad;

template<>
class Quad<void> : public virtual Surf {
public:
    Quad() {}
    virtual ~Quad() {}

    UInt numberOfFaces   () const { return 4; }
    UInt numberOfVertices() const { return 4; }

    UInt numberOfSideVertices(const UInt f = 0) const { return 2; }
};

template<class T>
class Quad : public virtual Surface<T>,
             public virtual Quad<void> {
public:
	Quad();
    virtual ~Quad();
};

typedef Quad<Real> QuadR;
typedef Quad<Int > QuadI;

#endif /* QUAD4_H_ */
