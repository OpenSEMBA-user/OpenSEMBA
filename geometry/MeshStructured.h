/*
 * MeshStructured.h
 *
 *  Created on: Mar 13, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_
#define SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_

#include "elements/ElementsGroup.h"
#include "layers/LayerGroup.h"
#include "Grid.h"

class MeshStructured:
        public CoordinateGroup<CoordI3>,
        public ElementsGroup<ElemI>,
        public LayerGroup<>,
        public Grid3 {
public:
    virtual ~MeshStructured();
    MeshStructured();
    MeshStructured(const Grid3& grid, const Mesh& mesh);

    vector<BoxR3> getRectilinearHexesInsideRegion(
            const ElementsGroup<ElemR>& region) const;

    vector<BoxR3> discretizeWithinBoundary(
            const UInt matId,
            const UInt layId) const;

    const Grid3* getGrid() const;
    void setGrid(const Grid3& grid_);

    ElementsGroup<NodeI> add(const ElementsGroup<NodeR>&);
    ElementsGroup<SurfI> add(const ElementsGroup<SurfR>&);
    ElementsGroup<VolI> add(const VolR&);
    ElementsGroup<VolI> add(const ElementsGroup<VolR>&);

    void applyScalingFactor(const Real factor);
    virtual void printInfo() const;
};

#endif /* SRC_COMMON_GEOMETRY_MESHSTRUCTURED_H_ */
