/*
 * MeshOpenfoam.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: luis
 */

#include "MeshOpenfoam.h"

MeshOpenfoam::MeshOpenfoam() {
}

MeshOpenfoam::MeshOpenfoam(
 const CoordinateGroup& cG,
 const vector<FaceIdentifier>& face,
 const vector<uint>& owner,
 const vector<uint>& neighbour,
 const vector<OpenfoamBoundary>& boundaries) {
	cG_ = cG;
	face_.resize(face.size());
	for (uint i = 0; i < face.size(); i++) {
		face_[i] = Polygon(cG_, face[i].first, face[i].second);
	}
	owner_ = owner;
	neighbour_ = neighbour;
	boundary_ = boundaries;
}

MeshOpenfoam::~MeshOpenfoam() {
}

vector<Polyhedron>
MeshOpenfoam::getPolyhedrons() const {
	vector<Polyhedron> res;
	// Determines number of cells.
	uint maxId = 0;
	for (uint i = 0; i < owner_.size(); i++) {
		if (owner_[i] > maxId) {
			maxId = owner_[i];
		}
	}
	for (uint i = 0; i < neighbour_.size(); i++) {
		if (neighbour_[i] > maxId) {
			maxId = neighbour_[i];
		}
	}
 	const uint nCells = maxId + 1;
 	// Stores info of faces that belong to each cell.
	vector<vector<uint> > cellFaces(nCells);
	for (uint i = 0; i < owner_.size(); i++) {
		cellFaces[owner_[i]].push_back(i);
	}
	for (uint i = 0; i < neighbour_.size(); i++) {
		cellFaces[neighbour_[i]].push_back(i);
	}
	// Stores polyhedrons.
	res.resize(nCells);
	for (uint c = 0; c < nCells; c++) {
		vector<const Polygon*> faces(cellFaces[c].size());
		for (uint f = 0; f < cellFaces[c].size(); f++) {
			faces[f] = &face_[cellFaces[c][f]];
		}
		res[c] = Polyhedron(c, faces);
	}
	return res;
}

vector<const Polygon*>
MeshOpenfoam::getBoundaryPolygons(
		const string& boundaryName) const {
	const OpenfoamBoundary *b = getBoundary(boundaryName);
	vector<const Polygon*> res;
	res.reserve(b->getFaces());
	const uint startFace = b->getStartFace();
	for (uint e = 0; e < b->getFaces(); e++) {
		res.push_back(&face_[startFace + e]);
	}
	return res;
}

const OpenfoamBoundary*
MeshOpenfoam::getBoundary(
		const string& boundaryName) const {
	for (uint i = 0; i < boundary_.size(); i++) {
		if (boundary_[i].getName().compare(boundaryName) == 0) {
			return &boundary_[i];
		}
	}
	cerr<< "ERROR @ MeshOpenfoam: "
		<< "Unable to find surface with name: "
		<< boundaryName;
	return NULL;
}

CVecD3
MeshOpenfoam::getClosestPointOnBoundary(const CVecD3 pos) const {
	CVecD3 res;
	double minDist = numeric_limits<double>::infinity();
	for (uint b = 0; b < boundary_.size(); b++) {
		for (uint f = 0; f < boundary_[b].getFaces(); f++) {
			const Polygon* surf = &face_[f + boundary_[b].getStartFace()];
			for (uint i = 0; i < surf->numberOfCoordinates(); i++) {
				CVecD3 candidate = surf->getV(i)->pos();
				if ((candidate - res).norm() < minDist) {
					res = candidate;
				}
			}
		}
	}
	return res;
}

void
MeshOpenfoam::printInfo() const {
	cout<< "--- Mesh Openfoam ---" << endl;
	cG_.printInfo();
	cout<< "- Number of faces: " << face_.size() << endl;
}

bool
MeshOpenfoam::isOnBoundary(const CVecD3 pos) const {
	const CoordD3* coord = cG_.get(pos);
	if (coord == NULL) {
		return false;
	} else {
		for (uint b = 0; b < boundary_.size(); b++) {
			for (uint f = 0; f < boundary_[b].getFaces(); f++) {
				uint id = f + boundary_[b].getStartFace();
				if (face_[id].isCoordinate(coord)) {
					return true;
				}
			}
		}
		return false;
	}
}

vector<const Polygon*>
MeshOpenfoam::getMaterialBoundary(
        const uint matId,
        const uint layId) const {
	vector<const Polygon*> res;
	for (uint i = 0; i < boundary_.size(); i++) {
		if (boundary_[i].isMaterial()
		        && boundary_[i].getMaterialId() == matId
		        && boundary_[i].getLayerId() == layId) {
			res.reserve(boundary_[i].getFaces());
			for (uint j = 0; j < boundary_[i].getFaces(); j++) {
				uint id = j + boundary_[i].getStartFace();
				res.push_back(&face_[id]);
			}
		}
	}
	return res;
}

vector<BoundingBox>
MeshOpenfoam::discretizeWithinBoundary(
 const RectilinearGrid& grid,
 const PhysicalModel* mat,
 const Layer* lay) const {
	return discretizeWithinBoundary(grid,
	        getMaterialBoundary(mat->getId(), lay->getId()));
}

void
MeshOpenfoam::addCoordinates(const RectilinearGrid& grid) {
	cG_.add(grid.getPos());
}

vector<BoundingBox>
MeshOpenfoam::discretizeWithinBoundary(
 const RectilinearGrid& grid,
 const vector<const Polygon*>& surf) const {
	// --- Preliminar checkings -----------------------------------------------
	checkAllFacesAreRectangular();
	// --- Prorcedures --------------------------------------------------------
	// Gets pairs of quads that define the volume of the space within them.
	const vector<pair<const Polygon*, const Polygon*> > pairs
	 = getPairsDefiningVolumeWithin(grid, surf);
	// Gets positions in z-axis.
	vector<BoundingBox> box(pairs.size());
	vector<vector<double> > zPos(pairs.size());
	for (uint p = 0; p < pairs.size(); p++) {
		CVecD3 min = pairs[p].first->getMinV()->pos();
		CVecD3 max = pairs[p].second->getMaxV()->pos();
		box[p] = BoundingBox(min,max);
		if (min(2) > max(2)) {
			zPos[p] = grid.getPosInRange(z, max(2), min(2));
		} else {
			zPos[p] = grid.getPosInRange(z, min(2), max(2));
		}
		assert(zPos[p].size() > 0);
	}
	// Fills the space within pairs with hexahedrons aligned with the grid.
	uint nHex = 0;
	for (uint p = 0; p < zPos.size(); p++) {
		nHex += zPos[p].size() - 1;
	}
	vector<BoundingBox> res;
	res.reserve(nHex);
	for (uint p = 0; p < pairs.size(); p++) {
		for (uint i = 1; i < zPos[p].size(); i++) {
			CVecD3 min(box[p].get_min()(0), box[p].get_min()(1), zPos[p][i-1]);
			CVecD3 max(box[p].get_max()(0), box[p].get_max()(1), zPos[p][i]);
			res.push_back(BoundingBox(min, max));
		}
	}
	return res;
}

void
MeshOpenfoam::checkAllFacesAreRectangular() const {
	for (uint i = 0; i < face_.size(); i++) {
		if (!face_[i].isRectangular()) {
			cerr<< "ERROR @ MeshOpenfoam: "
				<< "The following polygon is not a rectangular." << endl;
			face_[i].printInfo();
		}
	}
}

vector<pair<const Polygon*, const Polygon*> >
MeshOpenfoam::getPairsDefiningVolumeWithin(
 const RectilinearGrid& grid,
 const vector<const Polygon*>& origBound) const {
	vector<pair<const Polygon*, const Polygon*> > res;
	// --- Preliminar checkings ------------------------------------------------
	const uint nOrigBound = origBound.size();
	// Checks that bound.size is an even number.
	if (nOrigBound % 2 != 0) {
		cerr<< "ERROR @ Mesh:"
			<< "Bound size must be an even number to be closed." << endl;
		return res;
	}
	// Checks if boundary is belongs to natural faces of the grid.
	// The tolerance used is a fourth of the cell size.
	for (uint b = 0; b < nOrigBound; b++) {
		const uint nV = origBound[b]->numberOfVertices();
		vector<CVecD3> pos(nV);
		for (uint i = 0; i < nV; i++) {
			pos[i] = origBound[b]->getV(i)->pos();
		}
		static const double naturalCellTolerance = 0.25;
		if (!grid.isNaturalCell(pos, naturalCellTolerance)) {
			cerr<< "ERROR @ Mesh: "
				<< "Quads are not aligned with grid." << endl;
			return res;
		}
	}
	// --- Procedures ----------------------------------------------------------
	// Remove Surfaces not lying in a xy plane.
	vector<const Polygon*> bound;
	bound.reserve(nOrigBound);
	for (uint b = 0; b < origBound.size(); b++) {
		if (origBound[b]->isContainedInPlane(xy)) {
			bound.push_back(origBound[b]);
		}
	}
	// Sort remaining quad Ids using coordinates as a lexicographical order.
	// Pairs quadrilaterals that are aligned in the same Z natural axe.
	const uint nBound = bound.size();
	assert(nBound % 2 == 0);
	res.reserve(nBound / 2);
	// Sorts.
	DynMatrix<double> quads(nBound, 4);
	for (uint b = 0; b < nBound; b++) {
		CVecD3 minPos = bound[b]->getMinV()->pos();
		// Stores boundary at quad list.
		quads(b, 0) = (double) bound[b]->getId();
		for (uint i = 0; i < 3; i++) {
			quads(b, i+1) = minPos(i);
		}
	}
	quads.sortRows_omp(1,3);
	// Performs pairing.
	pair<const Polygon*, const Polygon*> aux;
	for (uint b = 0; b < nBound; b++) {
		const uint id = quads(b,0);
		if (b % 2 == 0) {
			aux.first = &face_[id];
		} else {
			aux.second = &face_[id];
			res.push_back(aux);
		}
	}
	return res;
}

