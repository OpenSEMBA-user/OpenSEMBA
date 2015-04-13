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

class LineBase : public virtual ElementBase {
public:
    LineBase() {}
    virtual ~LineBase() {}

    inline UInt numberOfFaces   () const { return 2; }
    inline UInt numberOfVertices() const { return 2; }

    inline UInt numberOfSideVertices   (const UInt f = 0) const { return 1; }
    inline UInt numberOfSideCoordinates(const UInt f = 0) const { return 1; }
};

template<class T>
class Line : public virtual Element<T>,
             public virtual LineBase {
public:
    Line();
    virtual ~Line();
};

typedef LineBase   Lin;
typedef Line<Real> LinR;
typedef Line<Int > LinI;

#endif /* LINE_H_ */
