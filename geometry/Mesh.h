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

class Mesh {
public:
    ElementsGroup<> elem_;
    CoordinateGroup<> cG_;
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
    void setMaterialIds(
            const vector<ElementId>& ids,
            const MatId newMatId);
    vector<ElementId> addAsHex8(const BoxR3& box);
    BoxR3 getBound(
            const vector<pair<const Volume*, UInt> >& border) const;
    BoxR3 getBound(
            const vector<ElementId>& list) const;
    vector<ElementId> getIdsInsideBound(
            const BoxR3& bound,
            const Element::Type type = Element::undefined) const;
    void applyGeometricScalingFactor(
            const Real factor);
    vector<ElementId> getTetIds(
            const vector<ElementId> elemIds) const;
    vector<UInt> getIdsOfCurvedTets() const;
    vector<const Tri3*> getTriWithMatId(
            const UInt matId, const bool ignoreTet = false) const;
    vector<const Tri3*> getTriWithId(const vector<ElementId>& Id) const;
    vector<pair<const Volume*, UInt> > getInternalBorder(
            const vector<ElementId>& elemIds) const;
    vector<pair<const Volume*, UInt> > getExternalBorder(
            const vector<ElementId>& elemIds) const;
    void linearize();
    bool isLinear() const;
    bool isRectilinear() const;
    const Grid3* getGrid() const;
    void setGrid(const Grid3& grid_);
    virtual vector<pair<const Element*, UInt> > getElementsWithVertex(
            const UInt vertexId,
            const Element::Type type) const;
    vector<BoxR3> getRectilinearHexesInsideRegion(
            const vector<const Element*>& region) const;
    pair<const Volume*, UInt> getBoundary(const Surface*) const;
    vector<UInt> getAdjacentElements(const vector<ElementId>& elemIds) const;
    vector<pair<const Tet*, UInt> > getBorderWithNormal(
            const vector<pair<const Tet*, UInt> >& border,
            const CVecR3& normal);
    bool isFloatingCoordinate(const CoordR3* coordinate) const;
    virtual bool isOnBoundary(const CVecR3 pos) const;
    virtual const CoordR3* getClosestVertex(const CVecR3 pos) const;
    virtual vector<const Surface*> getMaterialBoundary(
            const MatId   matId,
            const LayerId layId) const;
    virtual vector<BoxR3> discretizeWithinBoundary(
            const UInt matId,
            const UInt layId) const;
    pair<const Volume*, UInt> getNeighConnection(
            pair<const Volume*, const UInt> inner) const;
    virtual void printInfo() const;
protected:
    MapGroup map_;
    Grid3* grid_;
    static const Real areaDiffTolerance;
    vector<pair<const Volume*, UInt> >
    getInternalBorderOfTetRegion(
            const vector<ElementId>& region) const;
    vector<pair<const Volume*, UInt> >
    getInternalBorderOfTriRegion(
            const vector<ElementId>& region) const;
};


#endif /* MESH_H_ */
