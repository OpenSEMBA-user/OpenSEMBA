/*
 * Mesh.h
 *
 *  Created on: Mar 19, 2015
 *      Author: damarro
 */

#ifndef SRC_COMMON_GEOMETRY_MESH_H_
#define SRC_COMMON_GEOMETRY_MESH_H_

#include "Types.h"
#include "coordinates/CoordinateGroup.h"
#include "elements/ElementsGroup.h"
#include "layers/LayerGroup.h"
#include "Grid.h"

#include "ClassBase.h"

class Mesh : public ClassBase,
             public LayerGroup<>{
public:
    Mesh(const LayerGroup<>& layer = LayerGroup<>());
    Mesh(const Mesh& rhs);
    ~Mesh();

    Mesh& operator=(const Mesh& rhs);

    LayerGroup<>&       layers()       { return *this; }
    const LayerGroup<>& layers() const { return *this; }

    virtual void applyScalingFactor(const Real factor) = 0;

    virtual void printInfo() const;
};

#endif /* SRC_COMMON_GEOMETRY_MESH_H_ */
