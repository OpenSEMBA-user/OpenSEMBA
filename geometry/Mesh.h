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
#include "maps/MapGroup.h"
#include "Grid.h"

using namespace std;

class Mesh : public ElementsGroup<> {
public:
    Mesh(
            const CoordinateGroup<>& cG,
            const ElementsGroup<>& elem,
            const Grid3* grid);
    Mesh(
            const CoordinateGroup<>& cG,
            const Grid3* grid);
    Mesh();
    Mesh(Mesh& param);
    virtual ~Mesh();

    vector<ElementId> addAsHex8(const BoxR3& box);

    vector<Face> getInternalBorder(const ElementsGroup<>& region) const;
    vector<Face> getExternalBorder(const ElementsGroup<>& region) const;
    ElementsGroup<> getAdjacentRegion(const ElementsGroup<>& region);
    vector<BoxR3> getRectilinearHexesInsideRegion(
            const ElementsGroup<>& region) const;

    bool isRectilinear() const;

    const Grid3* getGrid() const;
    void setGrid(const Grid3& grid_);

    vector<const Tri3*> getTriWithMatId(const UInt matId, const bool ignoreTet) const;
    Face getBoundary(const Surface*) const;
    vector<Face> getBorderWithNormal(const vector<Face>& border, const CVecR3& normal);
    bool isFloatingCoordinate(const CoordR3* coordinate) const;
    virtual vector<pair<const Element*, UInt> > getElementsWithVertex(
            const UInt vertexId,
            const Element::Type type) const;
    virtual bool isOnBoundary(const CVecR3 pos) const;
    virtual vector<const Surface*> getMaterialBoundary(
            const MatId   matId,
            const LayerId layId) const;
    virtual vector<BoxR3> discretizeWithinBoundary(
            const UInt matId,
            const UInt layId) const;

    Face getNeighConnection(Face inner) const;
    void applyGeometricScalingFactor(const Real factor);
    virtual void printInfo() const;
protected:
    CoordinateGroup<> cG_;
    MapGroup map_;
    Grid3* grid_;
    static const Real areaDiffTolerance;
    vector<Face> getInternalBorderOfTet(const ElementsGroup<>& tet) const;
    vector<Face> getInternalBorderOfTri(const ElementsGroup<>& tri) const;
};


#endif /* MESH_H_ */
