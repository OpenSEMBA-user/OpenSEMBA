/*
 * Quad4.h
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#ifndef QUAD_H_
#define QUAD_H_

#include "Surface.h"

class QuadBase : public virtual SurfaceBase {
public:
    QuadBase() {}
    virtual ~QuadBase() {}

    UInt numberOfFaces   () const { return 4; }
    UInt numberOfVertices() const { return 4; }

    UInt numberOfSideVertices(const UInt f = 0) const { return 2; }
};

template<class T>
class Quad : public virtual Surface<T>,
             public virtual QuadBase {
public:
	Quad();
    virtual ~Quad();
};

typedef QuadBase   Qua;
typedef Quad<Real> QuaR;
typedef Quad<Int > QuaI;

#endif /* QUAD4_H_ */
