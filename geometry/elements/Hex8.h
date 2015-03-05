/*
 * Hex8.h
 *
 *  Created on: Sep 24, 2013
 *      Author: luis
 */

#ifndef HEX8_H_
#define HEX8_H_

#include "Volume.h"


namespace _Hex8_nspace{
	const double tolerance = 1.0e-15;
}

class Hex8 : public Volume {
public:
	Hex8();
	Hex8(
	 const CoordinateGroup<>&,
	 const unsigned int id_,
	 const unsigned int matId_,
	 const CoordinateId vId[8]);
	Hex8(
	 const CoordinateGroup<>& v,
	 const CVecD3& min,
	 const CVecD3& max,
	 const uint matId = 0);
	virtual ~Hex8();
	const Coordinate<double,3>*
	 getV(const unsigned int i) const {return v[i];}
	void
	 setV(const unsigned int i, const Coordinate<double,3>*);
	const Coordinate<double,3>*
	 getSideV(const unsigned int face, const unsigned int i) const;
	const Coordinate<double,3>*
	 getSideVertex(const unsigned int face, const unsigned int i) const;
	const Coordinate<double,3>*
	 getVertex(const unsigned int i) const {return v[i];}
	inline bool
	 isCurved() const {return false;}
	inline bool
	 isCurvedFace(const unsigned int face) const {return false;}
	inline bool
	 isQuadratic() const {return false;}
	inline unsigned int
	 numberOfFaces() const {return 6;}
	inline unsigned int
	 numberOfVertices() const {return 8;}
	inline unsigned int
	 numberOfCoordinates() const {return 8;}
	inline unsigned int
	 numberOfSideCoordinates(const uint f = 0) const {return 4;}
	inline unsigned int
	 numberOfSideVertices(const uint f = 0) const {return 4;}
	double
	 getAreaOfFace(const unsigned int face) const;
	bool
	 isRegular() const;
	void
	 printInfo() const;
private:
	const Coordinate<double,3>* v[8];
};

#endif /* HEX8_H_ */
