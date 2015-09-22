/*
 * Mesh.h
 *
 *  Created on: Mar 19, 2015
 *      Author: damarro
 */

#ifndef SRC_COMMON_GEOMETRY_MESH_H_
#define SRC_COMMON_GEOMETRY_MESH_H_

#include "Types.h"
#include "coordinates/GroupCoordinates.h"
#include "elements/GroupElements.h"
#include "layers/GroupLayers.h"
#include "Grid.h"

#include "base/class/ClassBase.h"

class Mesh : public virtual ClassBase {
public:
    Mesh();
    ~Mesh();

    virtual Real getMinimumSpaceStep() const = 0;
    virtual void applyScalingFactor(const Real factor) = 0;
    virtual BoxR3 getBoundingBox() const = 0;
};

#endif /* SRC_COMMON_GEOMETRY_MESH_H_ */
