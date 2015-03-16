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

template<typename E = ElementBase,
         typename C = CoordinateBase,
         typename L = Layer>
class Mesh : public CoordinateGroup<C>,
             public ElementsGroup<E>,
             public LayerGroup<L> {
public:
    Mesh();
    Mesh(const CoordinateGroup<C>& cG,
         const ElementsGroup<E>& elem,
         const LayerGroup<L>& layer = LayerGroup<L>());
    Mesh(const Mesh<E,C,L>& param);
    virtual ~Mesh();

    Mesh<E,C,L>& operator=(const Mesh<E,C,L>& rhs);

    template<typename E2 = ElementBase,
             typename C2 = CoordinateBase,
             typename L2 = Layer>
    Mesh<E2,C2,L2> getMeshOf();

    template<typename E2 = ElementBase,
             typename C2 = CoordinateBase,
             typename L2 = Layer>
    Mesh<E2,C2,L2> newMeshOf();

    CoordinateGroup<C>& coords() { return *this; }
    ElementsGroup<E>&   elems () { return *this; }
    LayerGroup<L>&      layers() { return *this; }

    const CoordinateGroup<C>& coords() const { return *this; }
    const ElementsGroup<E>&   elems () const { return *this; }
    const LayerGroup<L>&      layers() const { return *this; }

    vector<ElementId> addAsHex8(const BoxR3& box);
    virtual vector<BoxR3> getRectilinearHexesInsideRegion(
            const Grid3* grid,
            const ElementsGroup<VolR>& region) const;

    vector<Face> getBorderWithNormal(const vector<Face>& border,
                                     const CVecR3& normal);
    bool isFloatingCoordinate(const CoordR3* coordinate) const;

    ElementsGroup<Tri>* convertToTri(const ElementsGroup<E>& region,
                                    bool ignoreTets) const;
    vector<Face> getInternalBorder(const ElementsGroup<E>& region) const;
    vector<Face> getExternalBorder(const ElementsGroup<E>& region) const;
    ElementsGroup<E> getAdjacentRegion(const ElementsGroup<E>& region);

    virtual void printInfo() const;
protected:
    static const Real areaDiffTolerance;
    vector<Face> getInternalBorder(const ElementsGroup<Tet>& tet) const;
    vector<Face> getInternalBorder(const ElementsGroup<Tri>& tri) const;
};

#include "Mesh.hpp"

#endif /* MESH_H_ */
