/*
 * Mesh.h
 *
 *  Created on: Jul 23, 2013
 *      Author: luis
 */

#ifndef MESH_H_
#define MESH_H_
#include <limits>
#include <vector>
#include <utility>
#include <iostream>
#include "elements/ElementsGroup.h"
#include "layers/LayerGroup.h"

#include "Grid.h"
#include "maps/MapGroup.h"

using namespace std;

class Mesh : public LayerGroup<>, public ElementsGroup<> {
public:
    Mesh(
            const CoordinateGroup<>& cG,
            const ElementsGroup<>& elem,
            const LayerGroup<>& layer = LayerGroup<>());
    Mesh();
    Mesh(Mesh& param);
    virtual ~Mesh();

    CoordinateGroup<>& coords() { return cG_;   }
    ElementsGroup<>&   elems () { return *this; }
    LayerGroup<>&      layers() { return *this; }

    const CoordinateGroup<>& coords() const { return cG_;   }
    const ElementsGroup<>&   elems () const { return *this; }
    const LayerGroup<>&      layers() const { return *this; }

    vector<ElementId> addAsHex8(const BoxR3& box);
    virtual vector<BoxR3> getRectilinearHexesInsideRegion(
            const Grid3* grid,
            const ElementsGroup<ElemR>& region) const;
//    virtual vector<BoxR3> discretizeWithinBoundary(
//            const Grid3* grid,
//            const UInt matId,
//            const UInt layId) const = 0;

    vector<Face> getBorderWithNormal(
            const vector<Face>& border,
            const CVecR3& normal);
    bool isFloatingCoordinate(const CoordR3* coordinate) const;
    virtual bool isOnBoundary(const CVecR3 pos) const;
    ElementsGroup<SurfR> getMaterialBoundary(
            const MatId   matId,
            const LayerId layId) const;
    ElementsGroup<Tri>* convertToTri(
            const ElementsGroup<>& region,
            bool ignoreTets) const;
    vector<Face> getInternalBorder(const ElementsGroup<>& region) const;
    vector<Face> getExternalBorder(const ElementsGroup<>& region) const;
    ElementsGroup<> getAdjacentRegion(const ElementsGroup<>& region);

    virtual void applyScalingFactor(const Real factor);
    virtual void printInfo() const;
protected:
    CoordinateGroup<> cG_;
    static const Real areaDiffTolerance;
    vector<Face> getInternalBorder(const ElementsGroup<Tet>& tet) const;
    vector<Face> getInternalBorder(const ElementsGroup<Tri>& tri) const;
};


#endif /* MESH_H_ */
