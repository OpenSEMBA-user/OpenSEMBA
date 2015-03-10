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

class Surface : public Element {
public:
    Surface(const LayerId layerId = LayerId(0),
            const MatId   matId   = MatId(0));
    Surface(const ElementId id,
            const LayerId layerId = LayerId(0),
            const MatId   matId   = MatId(0));
    Surface(const Surface& rhs);
    virtual ~Surface();
    
	bool isContainedInPlane() const;
	bool isContainedInPlane(const CartesianPlane plane) const;
	
	virtual ElementBase::Type getType() const { return surface; }
    
    virtual CVecR3 getNormal() const;
	
    bool isRectangular() const;
	
    virtual void printInfo() const;

protected:
	static const Real curvatureTolerance;
};

#endif /* SURFACE_H_ */
