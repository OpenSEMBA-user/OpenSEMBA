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
    Line(const LayerId layerId = LayerId(0),
         const uint matId   = 0);
    Line(const ElementId id,
         const LayerId layerId = LayerId(0),
         const uint matId   = 0);
    Line(const Line& rhs);
    Line(const ElementId id, const Line& rhs);
    virtual ~Line();
    
    inline uint numberOfFaces   () const { return 2; }
    inline uint numberOfVertices() const { return 2; }
    
    inline uint numberOfSideVertices   (const uint f = 0) const { return 1; }
    inline uint numberOfSideCoordinates(const uint f = 0) const { return 1; }
    
    virtual ElementBase::Type getType() const { return line; }
};

#endif /* LINE_H_ */
