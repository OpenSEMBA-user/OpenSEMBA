/*
 * Volume.h
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#ifndef VOLUME_H_
#define VOLUME_H_

#include "Surface.h"
#include "math/Simplex.h"

#include "Element.h"

class VolumeBase : public virtual ElementBase {
public:
    VolumeBase() {};
    virtual ~VolumeBase() {};
};

template<class T>
class Volume : public virtual Element<T>,
               public virtual VolumeBase {
public:
	Volume();
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

typedef VolumeBase   Vol;
typedef Volume<Real> VolR;
typedef Volume<Int > VolI;

#endif /* VOLUME_H_ */
