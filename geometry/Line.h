/*
 * Line.h
 *
 *  Created on: May 13, 2013
 *      Author: luis
 */

#ifndef LINE_H_
#define LINE_H_

#include "Element.h"
#include "../math/SimplexLin.h"

using namespace std;

class Line : public Element {
public:
	Line();
	Line(
	 const uint id_,
	 const uint matId_,
	 const uint layerId_);
	virtual ~Line();
   virtual Element::Type
    getType() const {return LINE;}
	inline unsigned int
	 numberOfFaces() const {return 2;}
	inline unsigned int
	 numberOfVertices() const {return 2;}
	inline unsigned int
	 numberOfSideVertices(const uint f = 0) const {return 1;}
	inline unsigned int
 	 numberOfSideCoordinates(const uint f = 0) const {return 1;}
};

#endif /* LINE_H_ */
