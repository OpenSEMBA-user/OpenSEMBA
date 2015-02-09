/*
 * Quad4.h
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#ifndef QUAD4_H_
#define QUAD4_H_

#include "Quad.h"

class Quad4: public Quad {
public:
	Quad4();
	Quad4(
	 const CoordinateGroup&,
     const unsigned int id_,
     const unsigned int matId_,
	 const unsigned int vId[4]);
	virtual ~Quad4();
	Quad4&
	 operator=(const Quad4& rhs);
	bool
	 isQuadratic() const {return false;}
	bool
	 isCurved() const {return false;}
	unsigned int
	 numberOfCoordinates() const {return 4;}
	unsigned int
	 numberOfSideCoordinates(const uint f = 0) const {return 2;}
	const Coordinate<double,3>*
	 getV(const unsigned int i) const {return v[i];}
	void
	 setV(const unsigned int i, const Coordinate<double,3>*);
	const Coordinate<double,3>*
	 getVertex(const unsigned int i) const;
	const Coordinate<double,3>*
	 getSideV(const unsigned int face, const unsigned int i) const;
	const Coordinate<double,3>*
	 getSideVertex(const unsigned int face, const unsigned int i) const;
	void
	 printInfo() const;
	void
	 check() const;
private:
	const Coordinate<double,3>* v[4];
};

#endif /* QUAD4_H_ */
