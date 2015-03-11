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

template<class T>
class Surface : public Element<T> {
public:
    Surface(const LayerId layerId = LayerId(0),
            const MatId   matId   = MatId(0));
    Surface(const ElementId id,
            const LayerId layerId = LayerId(0),
            const MatId   matId   = MatId(0));
    Surface(const Surface<T>& rhs);
    virtual ~Surface();
    
	bool isContainedInPlane() const;
	bool isContainedInPlane(const CartesianPlane plane) const;
	
	virtual ElementBase::Type getType() const { return ElementBase::surface; }
    
    virtual CartesianVector<T,3> getNormal() const;
	
    bool isRectangular() const;
	
    virtual void printInfo() const;

protected:
	static const Real curvatureTolerance;
};

#include "Surface.hpp"

typedef Surface<Real> SurfR;
typedef Surface<Int > SurfI;

#endif /* SURFACE_H_ */
