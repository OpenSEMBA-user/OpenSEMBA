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
#include "Element.h"
#include "Map.h"
#include "BoundingBox.h"
#include "RectilinearGrid.h"
#include "../math/MathMatrix.h"

using namespace std;

#ifndef MESH_ERROR
#define MESH_ERROR 9559
#endif

class Mesh {
public:
	CoordinateGroup v;
	ElementsGroup elem;
	MapGroup map;
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
	pair<CVecD3,CVecD3>
	 getBound(
	  const vector<pair<const Tet*, unsigned int> >& border) const;
	pair<CVecD3,CVecD3>
	 getBound(
	  const vector<unsigned int>& list) const;
	vector<unsigned int>
	 getIdsInsideBound(
	  const pair<CVecD3,CVecD3>& bound) const;
	void
	 applyGeometricScalingFactor(
	  const double factor);
	void
	 linearize();
	bool
	 isLinear() const;
	bool
	 isRectilinear() const;
	RectilinearGrid
     getGrid() const;
	RectilinearGrid
	 getGridFromHexahedrons() const;
	void
	 setGrid(const RectilinearGrid& grid_);
	void
	 removeHexahedrons();
	bool
	 canExtractGrid() const;
protected:
	RectilinearGrid* grid_;
#ifdef __GXX_EXPERIMENTAL_CXX0X__
	static constexpr double areaDiffTolerance = 1e-15;
#else
	static const double areaDiffTolerance = 1e-15;
#endif
	virtual Element*
	 getElementWithId(unsigned int id);
	virtual const Element*
	 getElementWithId(unsigned int id) const;
private:
	// DEPRECTATED, should be in BoundingBox.
	pair<CVecD3,CVecD3>
	 shrinkBound(
 	  const pair<CVecD3,CVecD3>& bound,
	  const pair<CVecD3,CVecD3>& constraint) const;
	pair<CVecD3,CVecD3>
	 enlargeBound(
	  const pair<CVecD3,CVecD3>& bound,
	  const pair<CVecD3,CVecD3>& constraint) const;
	pair<CVecD3,CVecD3>
	 getInfinityBound() const;
};
#endif /* MESH_H_ */
