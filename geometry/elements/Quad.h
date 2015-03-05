/*
 * Quad4.h
 *
 *  Created on: Apr 10, 2014
 *      Author: luis
 */

#ifndef QUAD_H_
#define QUAD_H_

#include "Surface.h"

class Quad : public Surface {
public:
	Quad(const uint layerId = 0,
	     const uint matId   = 0);
    Quad(const ElementId id,
         const uint layerId = 0,
         const uint matId   = 0);
    Quad(const Quad& rhs);
    Quad(const ElementId id, const Quad& rhs);
    virtual ~Quad();
    
	uint numberOfFaces   () const { return 4; }
	uint numberOfVertices() const { return 4; }
    
	uint numberOfSideVertices(const uint f = 0) const { return 2; }
};

#endif /* QUAD4_H_ */
