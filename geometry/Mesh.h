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
    Mesh();
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
    virtual bool
    isOnBoundary(const CVecD3 pos) const = 0;
    virtual CVecD3
    getClosestPointOnBoundary(const CVecD3 pos) const = 0;
    virtual vector<const Polygon*>
    getMaterialBoundary(const uint matId, const uint layId) const = 0;
    virtual vector<BoxD3>
    discretizeWithinBoundary(
            const uint matId,
            const uint layerId) const = 0;
    void printInfo() const;
protected:
    Grid3* grid_;
    static const double areaDiffTolerance;
    virtual Element* getElementWithId(uint id);
};


#endif /* MESH_H_ */
