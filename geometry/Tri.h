#ifndef TRI_H_
#define TRI_H_

#include "Surface.h"
#include "../math/SimplexTri.h"

class Tri : public Surface {
public:
	Tri();
	Tri(const CVecD3& normal_,
     const uint id_ = 0,
     const uint matId_ = 0,
     const uint layerId_ = 0);
	virtual ~Tri();
	bool
	 isTri() const {return true;}
	void
	 getOrderedVerticesId(
	  unsigned int val[3]) const;
	unsigned int
	 numberOfFaces() const {return 3;}
	unsigned int
	 numberOfVertices() const {return 3;}
	unsigned int
	 numberOfSideVertices(const uint f = 0) const {return 2;}
};

#endif
