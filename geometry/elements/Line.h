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

template<class T = void>
class Line;

template<>
class Line<void> : public virtual Elem {
public:
    Line();
    virtual ~Line();

    inline UInt numberOfFaces   () const { return 2; }
    inline UInt numberOfVertices() const { return 2; }

    inline UInt numberOfSideVertices   (const UInt f = 0) const { return 1; }
    inline UInt numberOfSideCoordinates(const UInt f = 0) const { return 1; }
};

template<class T>
class Line : public virtual Element<T>,
             public virtual Line<void> {
public:
    Line();
    virtual ~Line();
};

typedef Line<Real> LineR;
typedef Line<Int > LineI;

#endif /* LINE_H_ */
