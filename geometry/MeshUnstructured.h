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
                         public virtual GroupCoordinates<CoordR3>,
                         public virtual GroupElements<ElemR>,
                         public virtual GroupLayers<Layer> {
public:
    MeshUnstructured();
    MeshUnstructured(const GroupCoordinates<const CoordR3>& cG,
                     const GroupElements<const ElemR>& elem,
                     const GroupLayers<const Layer>& layer = GroupLayers<>());
    MeshUnstructured(const MeshUnstructured& param);
    virtual ~MeshUnstructured();

    MeshUnstructured& operator=(const MeshUnstructured& rhs);

    DEFINE_CLONE(MeshUnstructured);

    GroupCoordinates<CoordR3>& coords() { return *this; }
    GroupElements<ElemR>&     elems () { return *this; }
    GroupLayers<Layer>&        layers() { return *this; }

    const GroupCoordinates<CoordR3>& coords() const { return *this; }
    const GroupElements<ElemR>&     elems () const { return *this; }
    const GroupLayers<Layer>&        layers() const { return *this; }

    MeshStructured* getMeshStructured(const Grid3& grid,
                                      const Real tol = Grid3::tolerance) const;

    vector<Face> getBorderWithNormal(const vector<Face>& border,
                                     const CVecR3& normal);

    bool isFloatingCoordinate(const CoordR3* coordinate) const;
    virtual bool isOnBoundary(const CVecR3 pos) const;
    GroupElements<const SurfR> getMaterialBoundary(const MatId   matId,
                                                   const LayerId layId) const;
    GroupElements<const SurfR> getSurfsMatching(
            const vector<Face>& faces) const;
    GroupElements<const Tri> convertToTri(
            const GroupElements<const ElemR>& region,
            bool ignoreTets) const;
    vector<Face> getInternalBorder(
            const GroupElements<const ElemR>& region) const;
    vector<Face> getExternalBorder(
            const GroupElements<const ElemR>& region) const;
    GroupElements<ElemR> getAdjacentRegion(
            const GroupElements<const ElemR>& region);

    Real getMinimumSpaceStep() const;
    void applyScalingFactor(const Real factor);
    BoxR3 getBoundingBox() const;

    void printInfo() const;
protected:
    vector<Face> getTetInternalBorder(
            const GroupElements<const Tet>& tet) const;
    vector<Face> getTriInternalBorder(
            const GroupElements<const Tri>& tri) const;
};

#endif /* MESHUNSTRUCTURED_H_ */
