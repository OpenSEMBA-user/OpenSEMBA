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

class SurfaceBase : public virtual ElementBase {
public:
    SurfaceBase() {};
    virtual ~SurfaceBase() {};
    virtual CVecR3 getNormalR() const {
        if (this->is<SurfR>()) {
            return this->castTo<SurfR>()->getNormal();
        } else {
            CVecR3 res;
            CVecI3 aux this->castTo<SurfI>()->getNormal();
            for (UInt d = 0; d < 3; d++) {
                res(d) = (Real) aux(d);
            }
            return res;
        }
    }
};

template<class T>
class Surface : public virtual Element<T>,
                public virtual SurfaceBase {
public:
    Surface();
    virtual ~Surface();

    bool isRectangular() const;
    bool isContainedInPlane() const;
    bool isContainedInPlane(const CartesianPlane plane) const;

    virtual CartesianVector<T,3> getNormal() const;

    virtual void printInfo() const;
};

typedef SurfaceBase   Surf;
typedef Surface<Real> SurfR;
typedef Surface<Int > SurfI;

#endif /* SURFACE_H_ */
