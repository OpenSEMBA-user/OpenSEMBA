/*
 * Surface.h
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#ifndef SURFACE_H_
#define SURFACE_H_

#include "Element.h"

using namespace std;

template<class T = void>
class Surface;

template<>
class Surface<void> : public virtual Elem {
public:
    Surface() {};
    virtual ~Surface() {};
};

template<class T>
class Surface : public virtual Element<T>,
                public virtual Surface<void> {
public:
    Surface();
    virtual ~Surface();

    bool isRectangular() const;
    bool isContainedInPlane() const;
    bool isContainedInPlane(const CartesianPlane plane) const;

    virtual CartesianVector<T,3> getNormal() const;
	
    virtual void printInfo() const;
};

typedef Surface<void> Surf;
typedef Surface<Real> SurfR;
typedef Surface<Int > SurfI;

#endif /* SURFACE_H_ */
