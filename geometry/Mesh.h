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
    ElementsGroup elem_;
    CoordinateGroup<> cG_;
    Mesh(
            const CoordinateGroup<>& cG,
            const ElementsGroup& elem,
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
    vector<ElementId> addAsHex8(const BoxD3& box);
    BoxD3 getBound(
            const vector<pair<const Volume*, uint> >& border) const;
    BoxD3 getBound(
            const vector<ElementId>& list) const;
    vector<uint> getIdsInsideBound(
            const BoxD3& bound,
            const Element::Type type = Element::undefined) const;
    void applyGeometricScalingFactor(
            const double factor);
    vector<uint> getTetIds(
            const vector<uint> elemIds) const;
    vector<uint> getIdsOfCurvedTets() const;
    vector<Tri3> getTriWithMatId(
            const uint matId, const bool ignoreTet = false) const;
    vector<Tri3> getTriWithId(const vector<uint>& Id) const;
    vector<pair<const Volume*, uint> > getInternalBorder(
            const vector<uint>& elemIds) const;
    vector<pair<const Volume*, uint> > getExternalBorder(
            const vector<uint>& elemIds) const;
    void linearize();
    bool isLinear() const;
    bool isRectilinear() const;
    const Grid3* getGrid() const;
    void setGrid(const Grid3& grid_);
    virtual vector<pair<const Element*, uint> > getElementsWithVertex(
            const uint vertexId,
            const Element::Type type) const;
    vector<BoxD3> getRectilinearHexesInsideRegion(
            const vector<const Element*>& region) const;
    pair<const Volume*, uint> getBoundary(const Surface*) const;
    vector<uint> getAdjacentElements(const vector<uint>& elemIds) const;
    vector<pair<const Tet*, uint> > getBorderWithNormal(
            const vector<pair<const Tet*, uint> >& border,
            const CVecD3& normal);
    bool isFloatingCoordinate(const CoordD3* coordinate) const;
    virtual bool isOnBoundary(const CVecD3 pos) const;
    virtual const CoordD3* getClosestVertex(const CVecD3 pos) const;
    virtual vector<const Surface*> getMaterialBoundary(
            const uint matId,
            const uint layId) const;
    virtual vector<BoxD3> discretizeWithinBoundary(
            const uint matId,
            const uint layId) const;
    pair<const Volume*, uint> getNeighConnection(
            pair<const Volume*, const uint> inner) const;
    virtual void printInfo() const;
protected:
    MapGroup map_;
    Grid3* grid_;
    static const double areaDiffTolerance;
    vector<pair<const Volume*, uint> >
    getInternalBorderOfTetRegion(
            const vector<uint>& region) const;
    vector<pair<const Volume*, uint> >
    getInternalBorderOfTriRegion(
            const vector<uint>& region) const;
};


#endif /* MESH_H_ */
