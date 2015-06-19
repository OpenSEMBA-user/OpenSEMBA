/*
 * CellGroup.cpp
 *
 *  Created on: Aug 29, 2012
 *      Author: luis
 */
#ifndef _CELLGROUP_H_
#include "CellGroup.h"
#endif

CellGroup::CellGroup(
 const SmbData* smb) {
	const MeshVolume* mesh = smb->mesh;
	const PMGroup* pMGroup = smb->pMGroup;
	uint nElem = mesh->elem_.tet.size();
	cell.resize(nElem, NULL);
	cellOffsetId = mesh->elem_.offsetIdTet;
	// Reserves space for cell vectors.
	vector<const Tet*> linear, quadratic;
	for (uint k = 0; k < nElem; k++) {
		const Tet* tet = mesh->elem_.tet[k];
		if (!tet->isCurved()) {
			linear.push_back(tet);
		} else {
			quadratic.push_back(tet);
		}
	}
	linTet.resize(linear.size(), CellTet4<ORDER_N>());
	quadTet.resize(quadratic.size(), CellTet10<ORDER_N>());
	for (uint k = 0; k < linear.size(); k++) {
		linTet[k] = CellTet4<ORDER_N>(linear[k], *pMGroup);
		cell[linTet[k].getId() - cellOffsetId] = &linTet[k];
	}
	for (uint k = 0; k < quadratic.size(); k++) {
		quadTet[k] = CellTet10<ORDER_N>(quadratic[k], *pMGroup);
		cell[quadTet[k].getId() - cellOffsetId] = &quadTet[k];
	}
	buildNodalMaps(mesh->map);
	check(mesh->map);
}
 
CellGroup::~CellGroup() {
	// TODO Auto-generated destructor stub
}
 
const CellTet<ORDER_N>*
CellGroup::operator()(const uint i) const {
	return cell[i];
}

const CellTet<ORDER_N>*
CellGroup::getPtrToCell(const Tet* elem) const {
	return getPtrToCellWithId(elem->getId());
}
 
const CellTet<ORDER_N>*
CellGroup::getPtrToCellWithId(const uint id) const {
	return cell[id - cellOffsetId];
}

void
CellGroup::buildNodalMaps(const MapGroup& map) {
	// PURPOSE:
	// - Creates two maps, mapP, and vmapP.
	// - mapP[f][n] stores the number of the node adjacent to the node n in
	//   face f. With a face node notation.
	// - vmapP[f][n] does the same with a node notation.
	CVecR3 diff, posM, posP;
	uint nK = cell.size();
	for (uint e = 0; e < nK; e++) {
	   const uint id = cell[e]->getId();
		for (uint f = 0; f < cell[e]->getFaces(); f++) {
			// Stores contiguous element (e2) number and orientation.
			const Tet* neigh = map.getNeighbour(id, f);
			const CellTet<ORDER_N>* c2 = getPtrToCell(neigh);
			const uint f2 = map.getVolToF(id, f);
			// Runs over each node in local element.
			for (uint i = 0; i < cell[e]->getNfp(); i++) {
				// Initializes mapP and vmapP to default values.
				cell[e]->vmapP[f][i] = cell[e]->getSideNode(f, i);
				// Creates the position vector of local element.
				posM = cell[e]->getSideNodePos(f, i);
				// Checks posM against all nodes in e2 face.
				for (uint j = 0; j < c2->nfp; j++) {
					posP = c2->getSideNodePos(f2, j);
					diff = posM - posP;
					// Stores value if the share position.
					if (diff.norm() < CELL_NODE_TOLERANCE)
						cell[e]->vmapP[f][i] = cell[e]->getSideNode(f2, j);
				}
			}
		}
	}
}
 
void
CellGroup::check(const MapGroup& map) const {
	if (cellOffsetId != cell[0]->getId()) {
		cerr << endl << "ERROR@CellGroup::check" << endl;
		exit(CELL_ERROR);
	}
	checkIdsAreConsecutive();
//	checkReciprocityInConnectivities();
	checkNodalMaps(map);
//	checkAreaCoherence();
}
 
void
CellGroup::checkIdsAreConsecutive() const {
	uint currentId = cellOffsetId;
	for (uint i = 1; i < cell.size(); i++) {
		if (cell[i]->getId() == currentId + 1) {
			currentId++;
		} else {
			cerr << endl << "ERROR@CellGroup::checkIdsAreConsecutive" << endl;
			exit(CELL_ERROR);
		}
	}
}

void
CellGroup::checkNodalMaps(const MapGroup& map) const {
	// Checks for vmap.
	CVecR3 diff;
	bool problem = false;
	uint nK = cell.size();
	for (uint e = 0; e < nK; e++) {
	   const uint id = cell[e]->getId();
		for (int f = 0; f < 4; f++) {
		   const Tet* neigh = map.getNeighbour(id, f);
			const CellTet<ORDER_N>* c2 = getPtrToCell(neigh);
			for (uint i = 0; i < cell[e]->getNfp(); i++) {
				int neighNode = cell[e]->vmapP[f][i];
				diff = cell[e]->getSideNodePos(f, i) - c2->n[neighNode];
				if (diff.norm() > CELL_NODE_TOLERANCE) {
					cerr << endl << "Elem " << e << ", face " << f << endl;
					problem = true;
				}
			}
		}
	}
	if (problem) {
		cerr << endl << "ERROR@CellGroup::checkNodalMaps()" << endl;
		cerr << endl << "vmapP contains errors. Check above." << endl;
		exit(CELL_ERROR);
	}
}

