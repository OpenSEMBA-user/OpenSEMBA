/*
 * MeshStructured.h
 *
 *  Created on: Mar 13, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_
#define SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_

#include "Mesh.h"
#include "Grid.h"

class MeshStructured: public Mesh, public Grid3 {
public:
    virtual ~MeshStructured();
    MeshStructured();

    vector<BoxR3> getRectilinearHexesInsideRegion(
            const ElementsGroup<ElemR>& region) const;

    vector<BoxR3> discretizeWithinBoundary(
            const UInt matId,
            const UInt layId) const;

    const Grid3* getGrid() const;
    void setGrid(const Grid3& grid_);

    void applyScalingFactor(const Real factor);
    virtual void printInfo() const;
};

#endif /* SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_ */
