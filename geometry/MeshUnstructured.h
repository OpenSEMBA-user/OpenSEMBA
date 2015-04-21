/*
 * Mesh.h
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */

#ifndef MESHUNSTRUCTURED_H_
#define MESHUNSTRUCTURED_H_

#include <iostream>
#include <limits>
#include <utility>
#include <vector>
using namespace std;

#include "maps/MapGroup.h"
#include "Grid.h"

#include "Mesh.h"

class MeshStructured;

class MeshUnstructured : public virtual Mesh,
                         public virtual CoordinateGroup<CoordR3>,
                         public virtual ElementsGroup<ElemR>,
                         public virtual LayerGroup<Layer> {
public:
    MeshUnstructured();
    MeshUnstructured(const CoordinateGroup<const CoordR3>& cG,
                     const ElementsGroup<const ElemR>& elem,
                     const LayerGroup<const Layer>& layer = LayerGroup<>());
    MeshUnstructured(const MeshUnstructured& param);
    virtual ~MeshUnstructured();

    MeshUnstructured& operator=(const MeshUnstructured& rhs);

    DEFINE_CLONE(MeshUnstructured);

    CoordinateGroup<CoordR3>& coords() { return *this; }
    ElementsGroup<ElemR>&     elems () { return *this; }
    LayerGroup<Layer>&        layers() { return *this; }

    const CoordinateGroup<CoordR3>& coords() const { return *this; }
    const ElementsGroup<ElemR>&     elems () const { return *this; }
    const LayerGroup<Layer>&        layers() const { return *this; }

    MeshStructured* getMeshStructured(const Grid3& grid,
                                      const Real tol = Grid3::tolerance) const;

    vector<Face> getBorderWithNormal(const vector<Face>& border,
                                     const CVecR3& normal);
    bool isFloatingCoordinate(const CoordR3* coordinate) const;
    virtual bool isOnBoundary(const CVecR3 pos) const;
    ElementsGroup<const SurfR> getMaterialBoundary(const MatId   matId,
                                                   const LayerId layId) const;
    ElementsGroup<const Tri> convertToTri(
            const ElementsGroup<const ElemR>& region,
            bool ignoreTets) const;
    vector<Face> getInternalBorder(
            const ElementsGroup<const ElemR>& region) const;
    vector<Face> getExternalBorder(
            const ElementsGroup<const ElemR>& region) const;
    ElementsGroup<ElemR> getAdjacentRegion(
            const ElementsGroup<const ElemR>& region);

    Real getMinimumSpaceStep() const;
    void applyScalingFactor(const Real factor);

    void printInfo() const;
protected:
    vector<Face> getTetInternalBorder(
            const ElementsGroup<const Tet>& tet) const;
    vector<Face> getTriInternalBorder(
            const ElementsGroup<const Tri>& tri) const;
};

#endif /* MESHUNSTRUCTURED_H_ */
