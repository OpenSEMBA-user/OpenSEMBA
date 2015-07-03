#ifndef TRI_H_
#define TRI_H_

#include "math/SimplexTri.h"

#include "Surface.h"

class Triangle : public Surface<Real> {
public:
	Triangle();
    virtual ~Triangle();
    
	UInt numberOfFaces   () const { return 3; }
	UInt numberOfVertices() const { return 3; }
    
	UInt numberOfSideVertices(const UInt f = 0) const { return 2; }
    
};

typedef Triangle Tri;

#endif
