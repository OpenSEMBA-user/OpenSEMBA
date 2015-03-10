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
            const LayerId layerId = LayerId(0),
            const MatId   matId   = MatId(0));
    Polygon(const Polygon& rhs);
	virtual ~Polygon();
    
    ClassBase* clone() const;
    
	UInt numberOfFaces      () const;
	UInt numberOfVertices   () const;
	UInt numberOfCoordinates() const;
    
	UInt numberOfSideVertices   (const UInt f) const;
	UInt numberOfSideCoordinates(const UInt f) const;
    
	const CoordR3* getV    (const UInt i) const;
	const CoordR3* getSideV(const UInt f,
                            const UInt i) const;
    
	const CoordR3* getVertex    (const UInt i) const;
	const CoordR3* getSideVertex(const UInt f,
                                 const UInt i) const;
    
	Real getArea() const;
	
	void setV(const UInt i, const CoordR3*);
    
    void printInfo() const;
private:
	vector<const CoordR3*> v_;
	// TODO void checkVertexCoplanarity() const;

};

#endif /* POLYGON_H_ */
