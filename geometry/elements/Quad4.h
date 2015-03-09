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
	Quad4(const CoordinateGroup<>&,
          const ElementId id,
          const CoordinateId vId[4],
          const LayerId layerId = LayerId(0),
          const uint matId   = 0);
    Quad4(const Quad4& rhs);
    Quad4(const ElementId id, const Quad4& rhs);
	virtual ~Quad4();
    
    ClassBase* clone()                   const;
    ClassBase* clone(const ElementId id) const;
    
	uint numberOfCoordinates() const { return 4; }
    
	uint numberOfSideCoordinates(const uint f = 0) const { return 2; }
	
    const CoordD3* getV    (const uint i) const { return v_[i]; }
	const CoordD3* getSideV(const uint f,
                            const uint i) const;
    
	const CoordD3* getVertex    (const uint i) const;
	const CoordD3* getSideVertex(const uint f,
                                 const uint i) const;
    
    void setV(const uint i, const CoordD3*);
    
	void printInfo() const;
	void check() const;

private:
	const CoordD3* v_[4];
};

#endif /* QUAD4_H_ */
