#ifndef TRI_H_
#define TRI_H_

#include "Surface.h"
#include "../math/SimplexTri.h"

class Tri : public Surface<Real> {
public:
	Tri(const LayerId layerId = LayerId(0),
	    const MatId   matId   = MatId(0));
    Tri(const ElementId id,
        const LayerId layerId = LayerId(0),
        const MatId   matId   = MatId(0));
    Tri(const Tri& rhs);
    virtual ~Tri();
    
	UInt numberOfFaces   () const { return 3; }
	UInt numberOfVertices() const { return 3; }
    
	UInt numberOfSideVertices(const UInt f = 0) const { return 2; }
    
    void getOrderedVerticesId(UInt val[3]) const;
};

#endif
