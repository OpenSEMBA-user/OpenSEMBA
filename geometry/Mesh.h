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
    CoordinateGroup cG_;
    Mesh(
            const CoordinateGroup& cG,
            const ElementsGroup& elem,
            const Grid3* grid);
    Mesh(
            const CoordinateGroup& cG,
            const Grid3* grid);
    Mesh();
    Mesh(Mesh& param);
    virtual
    ~Mesh();
    void
    setMaterialIds(
            const vector<unsigned int>& ids,
            const unsigned int newMatId);
    vector<unsigned int>
    getIdsWithMaterialId(const unsigned int matId) const;
    vector<unsigned int>
    getIdsWithoutMaterialId(const unsigned int matId) const;
    BoxD3
    getBound(
            const vector<pair<const Volume*, unsigned int> >& border) const;
    BoxD3
    getBound(
            const vector<unsigned int>& list) const;
    vector<unsigned int>
    getIdsInsideBound(
            const BoxD3& bound,
            const Element::Type type = Element::undefined) const;
    void
    applyGeometricScalingFactor(
            const double factor);
    vector<unsigned int>
    getTetIds(const vector<unsigned int> elemIds) const;
    vector<unsigned int>
    getIdsOfCurvedTets() const;
    vector<Tri3>
    getTriWithMatId(const uint matId, const bool ignoreTet = false) const;
    vector<Tri3> getTriWithId(const vector<uint>& Id) const;
    vector<pair<const Volume*, unsigned int> >
    getInternalBorder(const vector<unsigned int>& elemIds) const;
    vector<pair<const Volume*, unsigned int> >
    getExternalBorder(const vector<unsigned int>& elemIds) const;
    void
    linearize();
    bool
    isLinear() const;
    bool
    isRectilinear() const;
    const Grid3*
    getGrid() const;
    void
    setGrid(const Grid3& grid_);
    virtual vector<pair<const Element*, uint> >
    getElementsWithVertex(
            const uint vertexId,
            const Element::Type type) const;
    vector<BoxD3> getRectilinearHexesInsideRegion(
            const vector<const Element*>& region) const;
    pair<const Volume*, unsigned int>
    getBoundary(const Surface*) const;
    vector<unsigned int>
    getAdjacentElements(const vector<unsigned int>& elemIds) const;
    vector<pair<const Tet*, unsigned int> >
    getBorderWithNormal(
            const vector<pair<const Tet*, unsigned int> >& border,
            const CartesianVector<double,3>& normal);
    bool
    isFloatingCoordinate(const CoordD3* coordinate) const;
    virtual bool
    isOnBoundary(const CVecD3 pos) const;
    virtual const CoordD3*
    getClosestVertex(const CVecD3 pos) const;
    virtual vector<const Surface*>
    getMaterialBoundary(
            const uint matId,
            const uint layId) const;
    virtual vector<BoxD3>
    discretizeWithinBoundary(
            const uint matId,
            const uint layId) const;
    pair<const Volume*, unsigned int> getNeighConnection(
            pair<const Volume*, const unsigned int> inner) const;
    virtual void printInfo() const;
protected:
    MapGroup map;
    vector<pair<const Volume*, unsigned int> >
    getInternalBorderOfTetRegion(
            const vector<unsigned int>& region) const;
    vector<pair<const Volume*, unsigned int> >
    getInternalBorderOfTriRegion(
            const vector<unsigned int>& region) const;
    Grid3* grid_;
    static const double areaDiffTolerance;
};


#endif /* MESH_H_ */
