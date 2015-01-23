/*
 * Polygon.h
 *
 *  Created on: Jul 25, 2014
 *      Author: luis
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "Surface.h"
#include <stdlib.h>

class Polygon: public Surface {
public:
	Polygon();
	Polygon(
	 const CoordinateGroup& cG,
	 const uint id,
	 const vector<uint>& vId,
	 const uint matId = 0);
	virtual ~Polygon();
	unsigned int
	 numberOfCoordinates() const;
	unsigned int
	 numberOfVertices() const;
	unsigned int
	 numberOfFaces() const;
	unsigned int
	 numberOfSideVertices(const uint face) const;
	unsigned int
	 numberOfSideCoordinates(const uint face) const;
	void
	 setV(const unsigned int i, const Coordinate<double,3>*);
	const Coordinate<double,3>*
	 getV(const unsigned int i) const;
	const Coordinate<double,3>*
	 getSideV(const unsigned int f, const unsigned int i) const;
	const Coordinate<double,3>*
	 getVertex(const unsigned int i) const;
	const Coordinate<double,3>*
	 getSideVertex(const unsigned int f, const unsigned int i) const;
	bool
	 isCurved() const;
	double
	 getArea() const;
	void
	 printInfo() const;
private:
	vector<const Coordinate<double,3>*> v_;
	// TODO void checkVertexCoplanarity() const;

};

#endif /* POLYGON_H_ */
