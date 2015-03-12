/*
 * Volume.h
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#ifndef VOLUME_H_
#define VOLUME_H_

#include "Element.h"
#include "Surface.h"
#include "../math/Simplex.h"

template<class T>
class Volume : public Element<T> {
public:
	Volume(const LayerId layerId = LayerId(0),
           const MatId   matId   = MatId(0));
	Volume(const ElementId id,
           const LayerId layerId = LayerId(0),
           const MatId   matId   = MatId(0));
    Volume(const Volume<T>& rhs);
    virtual ~Volume();
    
	bool isLocalFace(const UInt f,
                     const Surface<T>& surf) const;
	virtual bool isCurvedFace(const UInt face) const = 0;
    virtual bool isFaceContainedInPlane(const UInt face,
                                        const CartesianPlane plane) const;
    
    CartesianVector<T,3> sideNormal(const UInt f) const;
   
    virtual Real getAreaOfFace(const UInt face) const = 0;
    Box<T,3> getBoundOfFace(const UInt face) const;
	UInt getFaceNumber(const Surface<T>*) const;
};

#include "Volume.hpp"

typedef Volume<Real> VolR;
typedef Volume<Int > VolI;

#endif /* VOLUME_H_ */
