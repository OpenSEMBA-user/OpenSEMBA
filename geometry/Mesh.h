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
#include "MapGroup.h"
#include "Grid.h"

using namespace std;

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
	void
	 removeHexahedrons();
	bool
	 canExtractGrid() const;
   virtual const Element*
    getElementWithId(unsigned int id) const;
   virtual vector<pair<const Element*, uint> >
    getElementsWithVertex(
          const uint vertexId,
          const Element::Type type) const;
protected:
	Grid3* grid_;
	static const double areaDiffTolerance;
	virtual Element*
	 getElementWithId(unsigned int id);
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


#endif /* MESH_H_ */
