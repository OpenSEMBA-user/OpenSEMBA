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
         const MatId   matId   = MatId(0));
    Line(const ElementId id,
         const LayerId layerId = LayerId(0),
         const MatId   matId   = MatId(0));
    Line(const Line& rhs);
    virtual ~Line();
    
    inline UInt numberOfFaces   () const { return 2; }
    inline UInt numberOfVertices() const { return 2; }
    
    inline UInt numberOfSideVertices   (const UInt f = 0) const { return 1; }
    inline UInt numberOfSideCoordinates(const UInt f = 0) const { return 1; }
    
    virtual ElementBase::Type getType() const { return line; }
};

#endif /* LINE_H_ */
