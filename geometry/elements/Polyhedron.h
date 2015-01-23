/*
 * Polyhedron.h
 *
 *  Created on: Jul 24, 2014
 *      Author: luis
 */

#ifndef POLYHEDRON_H_
#define POLYHEDRON_H_

#include "Volume.h"
#include "Polygon.h"

class Polyhedron: public Volume {
public:
	Polyhedron();
	Polyhedron(
	 const uint id,
	 const vector<const Polygon*>& faces);
	virtual ~Polyhedron();

	const Polygon*
	 getFace(const uint f) const;
	unsigned int
	 numberOfCoordinates() const;
	unsigned int
	 numberOfVertices() const;
	unsigned int
	 numberOfFaces() const;
	unsigned int
	 numberOfSideVertices(
	  const uint face = 0) const;
	unsigned int
	 numberOfSideCoordinates(
 	  const uint face = 0) const;
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
	bool
	 isCurvedFace(const unsigned int face) const;
	double
	 getAreaOfFace(const unsigned int face) const;
	void
	 printInfo() const;

private:
	vector<const Coordinate<double,3>*> v_;
	vector<const Polygon*> face_;
	void
	 addV(const Coordinate<double,3>*);
	void
	 checkClosedness() const;
};

#endif /* POLYHEDRON_H_ */
