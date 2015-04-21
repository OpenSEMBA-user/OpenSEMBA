#ifndef TRI_H_
#define TRI_H_

#include "math/SimplexTri.h"

#include "Surface.h"

class Tri : public Surface<Real> {
public:
	Tri();
    virtual ~Tri();
    
	UInt numberOfFaces   () const { return 3; }
	UInt numberOfVertices() const { return 3; }
    
	UInt numberOfSideVertices(const UInt f = 0) const { return 2; }
    
    void getOrderedVerticesId(UInt val[3]) const;
};

#endif
