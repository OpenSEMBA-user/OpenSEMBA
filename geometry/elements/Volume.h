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
           const uint matId   = 0);
	Volume(const ElementId id,
           const LayerId layerId = LayerId(0),
           const uint matId   = 0);
    Volume(const Volume& rhs);
    Volume(const ElementId id, const Volume& rhs);
    virtual ~Volume();
    
	bool isLocalFace(const uint f,
                     const Surface& surf) const;
	virtual bool isCurvedFace(const uint face) const = 0;
    virtual bool isFaceContainedInPlane(const uint face,
                                        const CartesianPlane plane) const;
    
	CVecD3 sideNormal(const uint f) const;
   
    ElementBase::Type getType() const {return volume;}
	
    virtual double getAreaOfFace(const uint face) const = 0;
    BoxD3 getBoundOfFace(const uint face) const;
	uint getFaceNumber(const Surface*) const;
};

#endif /* VOLUME_H_ */
