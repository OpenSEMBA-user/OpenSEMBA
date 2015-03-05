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
	Polygon(const CoordinateGroup<>& cG,
            const ElementId id,
            const vector<CoordinateId>& vId,
            const uint layerId = 0,
            const uint matId   = 0);
    Polygon(const Polygon& rhs);
    Polygon(const ElementId id, const Polygon& rhs);
	virtual ~Polygon();
    
    ClassBase* clone()                   const;
    ClassBase* clone(const ElementId id) const;
    
	uint numberOfFaces      () const;
	uint numberOfVertices   () const;
	uint numberOfCoordinates() const;
    
	uint numberOfSideVertices   (const uint f) const;
	uint numberOfSideCoordinates(const uint f) const;
    
	const CoordD3* getV    (const uint i) const;
	const CoordD3* getSideV(const uint f,
                            const uint i) const;
    
	const CoordD3* getVertex    (const uint i) const;
	const CoordD3* getSideVertex(const uint f,
                                 const uint i) const;
    
	double getArea() const;
	
	void setV(const uint i, const CoordD3*);
    
    void printInfo() const;
private:
	vector<const CoordD3*> v_;
	// TODO void checkVertexCoplanarity() const;

};

#endif /* POLYGON_H_ */
