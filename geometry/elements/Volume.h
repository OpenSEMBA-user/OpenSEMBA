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

class Volume : public Element {
public:
	Volume(const LayerId layerId = LayerId(0),
           const MatId   matId   = MatId(0));
	Volume(const ElementId id,
           const LayerId layerId = LayerId(0),
           const MatId   matId   = MatId(0));
    Volume(const Volume& rhs);
    virtual ~Volume();
    
	bool isLocalFace(const UInt f,
                     const Surface& surf) const;
	virtual bool isCurvedFace(const UInt face) const = 0;
    virtual bool isFaceContainedInPlane(const UInt face,
                                        const CartesianPlane plane) const;
    
	CVecR3 sideNormal(const UInt f) const;
   
    ElementBase::Type getType() const {return volume;}
	
    virtual Real getAreaOfFace(const UInt face) const = 0;
    BoxR3 getBoundOfFace(const UInt face) const;
	UInt getFaceNumber(const Surface*) const;
};

#endif /* VOLUME_H_ */
