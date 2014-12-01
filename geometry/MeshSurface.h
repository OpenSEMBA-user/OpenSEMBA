/*
 * MeshSurface.h
 *
 *  Created on: Jul 22, 2013
 *      Author: luis
 */

#ifndef MESHSURFACE_H_
#define MESHSURFACE_H_

#include <vector>
#include <assert.h>
#include "../math/CartesianVector.h"
#include "Mesh.h"

using namespace std;

class MeshSurface : public Mesh {
public:
	MeshSurface(
     const vector<CartesianVector<double,3> >& normals,
     const CoordinateGroup coordinates,
     const unsigned int numberOfElements
    );
	virtual
	~MeshSurface();
};

#endif /* MESHSURFACE_H_ */
