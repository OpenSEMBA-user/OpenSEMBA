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
	Quad();
	Quad(uint id, uint matId);
	virtual ~Quad();
	unsigned int
	 numberOfFaces() const {return 4;}
	unsigned int
	 numberOfVertices() const {return 4;}
	unsigned int
	 numberOfSideVertices(const uint f = 0) const {return 2;}
};

#endif /* QUAD4_H_ */
