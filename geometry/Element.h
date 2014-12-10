#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <limits>

#include "../math/MathMatrix.h"
#include "../math/CartesianVector.h"
#include "CoordinateGroup.h"
#include "BoundingBox.h"
#include <iostream>
#include <vector>

using namespace std;

#ifndef ELEMENT_ERROR
#define ELEMENT_ERROR 37311347
#endif

class Element {
public:
	typedef enum{
		undefined,
		NODE,
		LINE,
		SURFACE,
		VOLUME
	} Type;
	Element();
	Element(const uint id, const uint matId, const uint layerId);
	virtual ~Element();
	virtual bool
	 isQuadratic() const {return false;}
	virtual bool
	 isCurved() const = 0;
	virtual unsigned int
	 numberOfCoordinates() const = 0;
	virtual unsigned int
	 numberOfVertices() const = 0;
	virtual unsigned int
	 numberOfFaces() const = 0;
	virtual unsigned int
	 numberOfSideVertices(
	  const uint f = 0) const = 0;
	virtual unsigned int
	 numberOfSideCoordinates(
	  const uint f = 0) const = 0;
	virtual bool
	 isTri() const {return false;}
	virtual bool
	 isTet() const {return false;}
	virtual void
	 setV(const unsigned int i, const CoordD3*);
	virtual void
	 setMatId(const unsigned int newMatId);
	virtual const CoordD3*
	 getV(const unsigned int i) const = 0;
	virtual const CoordD3*
	 getSideV(const unsigned int f, const unsigned int i) const = 0;
	unsigned int
	 getId() const {return id;}
	unsigned int
	 getMatId() const {return matId;}
	unsigned int
	 getLayerId() const {return layerId;}
	virtual const CoordD3*
	 getVertex(const unsigned int i) const = 0;
	virtual const CoordD3*
	 getSideVertex(const unsigned int f, const unsigned int i) const = 0;
	BoundingBox
	 getBound() const;
	bool
	 isCoordinate(const CoordD3* coord) const;
	// Returns ptr to coord with min(max) lexicographical position.
	virtual const CoordD3*
	 getMinV() const;
	virtual const CoordD3*
	 getMaxV() const;
	virtual bool
	 isInnerPoint(const CVecD3& pos) const;
	virtual void
	 printInfo() const;
protected:
	unsigned int id;
	unsigned int matId;
	unsigned int layerId;
	static const double tolerance = 1e-15;
	void
	 ascendingOrder(unsigned int nVal, unsigned int* val) const;
	pair<CVecD3, CVecD3>
	 getInfinityBound() const;
};

#endif
