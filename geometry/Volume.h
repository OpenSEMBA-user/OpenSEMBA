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
	Volume();
	virtual ~Volume();
	bool isLocalFace(
	 const unsigned int f,
	 const Surface& surf) const;
	virtual bool
	 isCurved() const = 0;
	virtual bool
	 isCurvedFace(const unsigned int face) const = 0;
	CartesianVector<double,3>
	 sideNormal(const unsigned int f) const;
	virtual unsigned int
	 numberOfCoordinates() const = 0;
	virtual unsigned int
	 numberOfVertices() const = 0;
	virtual unsigned int
	 numberOfFaces() const = 0;
	virtual double
	 getAreaOfFace(const unsigned int face) const = 0;
	unsigned int
	 getFaceNumber(const Surface*) const;
	virtual void
	 printInfo() const = 0;
};

#endif /* VOLUME_H_ */
