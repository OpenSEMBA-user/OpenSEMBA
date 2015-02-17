/*
 * MeshOpenfoam.h
 *
 *  Created on: Jul 25, 2014
 *      Author: luis
 */

#ifndef MESHOPENFOAM_H_
#define MESHOPENFOAM_H_

#include "../../common/geometry/elements/Polyhedron.h"
#include "../../common/inputs/physicalModel/PhysicalModel.h"
#include "../../common/inputs/Layer.h"
#include "../geometry/Grid.h"
#include "OpenfoamBoundary.h"

typedef pair<uint, vector<uint> > FaceIdentifier;

class MeshOpenfoam {
public:
	MeshOpenfoam();
	MeshOpenfoam(
	 const CoordinateGroup& cG,
	 const vector<FaceIdentifier>& faceIndentifiers,
	 const vector<uint>& faceOwner,
	 const vector<uint>& faceNeighbour,
	 const vector<OpenfoamBoundary>& boundaries);
	virtual ~MeshOpenfoam();
	vector<Polyhedron>
	 getPolyhedrons() const;
	vector<const Polygon*>
	 getBoundaryPolygons(
			 const string& boundaryName) const;
	const OpenfoamBoundary*
	 getBoundary(
			 const string& boundaryName) const;
	bool
	 isOnBoundary(const CVecD3 pos) const;
	CVecD3
	 getClosestPointOnBoundary(const CVecD3 pos) const;
	void
	 printInfo() const;
	vector<const Polygon*>
	 getMaterialBoundary(const uint matId, const uint layId) const;
	vector<BoxD3>
	 discretizeWithinBoundary(
	  const Grid3& grid,
	  const PhysicalModel* mat,
	  const Layer* lay) const;
	void
	 addCoordinates(const Grid3& grid);
	void
	 checkAllFacesAreRectangular() const;
	bool areFacesRectangular() const;
private:
	CoordinateGroup cG_;
	vector<Polygon> face_;
	vector<uint> owner_;
	vector<uint> neighbour_;
	vector<OpenfoamBoundary> boundary_;
	vector<BoxD3>
	discretizeWithinBoundary(
			const Grid3& grid,
			const vector<const Polygon*>& faces) const;
	// Returns pairs of surface in boundary define a volume in the region
	// contained within them.
	vector<pair<const Polygon*, const Polygon*> >
	getPairsDefiningVolumeWithin(
			const Grid3& grid,
			const vector<const Polygon*>& boundary) const;
};

#endif /* MESHOPENFOAM_H_ */
