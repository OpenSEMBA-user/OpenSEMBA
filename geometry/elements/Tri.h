#ifndef TRI_H_
#define TRI_H_

#include "Surface.h"
#include "../math/SimplexTri.h"

class Tri : public Surface {
public:
	Tri(const uint layerId = 0,
	    const uint matId   = 0);
    Tri(const ElementId id,
        const uint layerId = 0,
        const uint matId   = 0);
    Tri(const Tri& rhs);
    Tri(const ElementId id, const Tri& rhs);
    virtual ~Tri();
    
	uint numberOfFaces   () const { return 3; }
	uint numberOfVertices() const { return 3; }
    
	uint numberOfSideVertices(const uint f = 0) const { return 2; }
    
    void getOrderedVerticesId(uint val[3]) const;
};

#endif
