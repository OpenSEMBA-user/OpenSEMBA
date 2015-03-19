/*
 * Mesh.h
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */

#ifndef MESHUNSTRUCTURED_H_
#define MESHUNSTRUCTURED_H_

#include <limits>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

#include "maps/MapGroup.h"

#include "Mesh.h"

class MeshUnstructured : public virtual Mesh,
                         public CoordinateGroup<CoordR3>,
                         public ElementsGroup<ElemR> {
public:
    MeshUnstructured();
    MeshUnstructured(const CoordinateGroup<CoordR3>& cG,
                     const ElementsGroup<ElemR>& elem,
                     const LayerGroup<>& layer = LayerGroup<>());
    MeshUnstructured(const MeshUnstructured& param);
    virtual ~MeshUnstructured();

    MeshUnstructured& operator=(const MeshUnstructured& rhs);

    virtual ClassBase* clone() const;

    CoordinateGroup<CoordR3>& coords() { return *this; }
    ElementsGroup<ElemR>&     elems () { return *this; }

    const CoordinateGroup<CoordR3>& coords() const { return *this; }
    const ElementsGroup<ElemR>&     elems () const { return *this; }

    vector<ElementId> addAsHex8(const BoxR3& box);
    virtual vector<BoxR3> getRectilinearHexesInsideRegion(
            const Grid3* grid,
            const ElementsGroup<VolR>& region) const;
//    virtual vector<BoxR3> discretizeWithinBoundary(
//            const Grid3* grid,
//            const UInt matId,
//            const UInt layId) const = 0;

    vector<Face> getBorderWithNormal(const vector<Face>& border,
                                     const CVecR3& normal);
    bool isFloatingCoordinate(const CoordR3* coordinate) const;
    virtual bool isOnBoundary(const CVecR3 pos) const;
    ElementsGroup<SurfR> getMaterialBoundary(const MatId   matId,
                                             const LayerId layId) const;
    ElementsGroup<Tri> convertToTri(const ElementsGroup<ElemR>& region,
                                    bool ignoreTets) const;
    vector<Face> getInternalBorder(const ElementsGroup<ElemR>& region) const;
    vector<Face> getExternalBorder(const ElementsGroup<ElemR>& region) const;
    ElementsGroup<ElemR> getAdjacentRegion(const ElementsGroup<ElemR>& region);

    void applyScalingFactor(const Real factor);

    void printInfo() const;
protected:
    vector<Face> getInternalBorder(const ElementsGroup<Tet>& tet) const;
    vector<Face> getInternalBorder(const ElementsGroup<Tri>& tri) const;
};

#endif /* MESHUNSTRUCTURED_H_ */
