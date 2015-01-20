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
#include "Grid.h"

using namespace std;

#ifndef MESH_ERROR
#define MESH_ERROR 9559
#endif

class Mesh {
public:
	CoordinateGroup v;
	ElementsGroup elem;
	MapGroup map;
    void
     addCoordinates(const Grid3& grid);
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
	  const vector<pair<const Tet*, unsigned int> >& border) const;
	BoxD3
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
	const Grid3*
     getGrid() const;
	Grid3
	 getGridFromHexahedrons() const;
	void
	 setGrid(const Grid3& grid_);
	void
	 removeHexahedrons();
	bool
	 canExtractGrid() const;
protected:
	Grid3* grid_;
	static const double areaDiffTolerance;
	virtual Element*
	 getElementWithId(unsigned int id);
	virtual const Element*
	 getElementWithId(unsigned int id) const;
private:
//	// DEPRECTATED, should be in BoundingBox.
//	pair<CVecD3,CVecD3>
//	 shrinkBound(
// 	  const pair<CVecD3,CVecD3>& bound,
//	  const pair<CVecD3,CVecD3>& constraint) const;
//	pair<CVecD3,CVecD3>
//	 enlargeBound(
//	  const pair<CVecD3,CVecD3>& bound,
//	  const pair<CVecD3,CVecD3>& constraint) const;
//	pair<CVecD3,CVecD3>
//	 getInfinityBound() const;
};

const double Mesh::areaDiffTolerance = 1e-15;

#endif /* MESH_H_ */
