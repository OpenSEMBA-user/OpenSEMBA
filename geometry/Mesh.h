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

#include "maps/MapGroup.h"

using namespace std;

class Mesh : public CoordinateGroup<CoordR3>,
             public ElementsGroup<ElemR>,
             public LayerGroup<> {
public:
    Mesh();
    Mesh(const CoordinateGroup<CoordR3>& cG,
         const ElementsGroup<ElemR>& elem,
         const LayerGroup<>& layer = LayerGroup<>());
    Mesh(const Mesh& param);
    virtual ~Mesh();

    Mesh& operator=(const Mesh& rhs);

    CoordinateGroup<CoordR3>& coords() { return *this; }
    ElementsGroup<ElemR>&     elems () { return *this; }
    LayerGroup<>&             layers() { return *this; }

    const CoordinateGroup<CoordR3>& coords() const { return *this; }
    const ElementsGroup<ElemR>&     elems () const { return *this; }
    const LayerGroup<>&             layers() const { return *this; }

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

    virtual void printInfo() const;
protected:
    static const Real areaDiffTolerance;
    vector<Face> getInternalBorder(const ElementsGroup<Tet>& tet) const;
    vector<Face> getInternalBorder(const ElementsGroup<Tri>& tri) const;
};

#endif /* MESH_H_ */
