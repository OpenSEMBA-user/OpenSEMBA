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
	UInt nElem = mesh->elems().sizeOf<Tet>();
	cell.resize(nElem, NULL);
	cellOffsetId = mesh->elem_.offsetIdTet;
	// Reserves space for cell vectors.
	vector<const Tet*> linear, quadratic;
	for (UInt k = 0; k < nElem; k++) {
		const Tet* tet = mesh->elem_.tet[k];
		if (!tet->isCurved()) {
			linear.push_back(tet);
		} else {
			quadratic.push_back(tet);
		}
	}
	linTet.resize(linear.size(), CellTet4<ORDER_N>());
	quadTet.resize(quadratic.size(), CellTet10<ORDER_N>());
	for (UInt k = 0; k < linear.size(); k++) {
		linTet[k] = CellTet4<ORDER_N>(linear[k], *pMGroup);
		cell[linTet[k].getId() - cellOffsetId] = &linTet[k];
	}
	for (UInt k = 0; k < quadratic.size(); k++) {
		quadTet[k] = CellTet10<ORDER_N>(quadratic[k], *pMGroup);
		cell[quadTet[k].getId() - cellOffsetId] = &quadTet[k];
	}
	buildNodalMaps(mesh->map);
	check(mesh->map);
}
 
CellGroup::~CellGroup() {
	// TODO Auto-generated destructor stub
}
 
const CellTet<ORDER_N>* CellGroup::operator()(const UInt i) const {
	return cell[i];
}

const CellTet<ORDER_N>* CellGroup::getPtrToCell(const Tet* elem) const {
	return getPtrToCellWithId(elem->getId());
}
 
const CellTet<ORDER_N>* CellGroup::getPtrToCellWithId(const UInt id) const {
	return cell[id - cellOffsetId];
}

void CellGroup::buildNodalMaps(const MapGroup& map) {
	// PURPOSE:
	// - Creates two maps, mapP, and vmapP.
	// - mapP[f][n] stores the number of the node adjacent to the node n in
	//   face f. With a face node notation.
	// - vmapP[f][n] does the same with a node notation.
	CVecR3 diff, posM, posP;
	UInt nK = cell.size();
	for (UInt e = 0; e < nK; e++) {
	   const UInt id = cell[e]->getId();
		for (UInt f = 0; f < cell[e]->getFaces(); f++) {
			// Stores contiguous element (e2) number and orientation.
			const Tet* neigh = map.getNeighbour(id, f);
			const CellTet<ORDER_N>* c2 = getPtrToCell(neigh);
			const UInt f2 = map.getVolToF(id, f);
			// Runs over each node in local element.
			for (UInt i = 0; i < cell[e]->getNfp(); i++) {
				// Initializes mapP and vmapP to default values.
				cell[e]->vmapP[f][i] = cell[e]->getSideNode(f, i);
				// Creates the position vector of local element.
				posM = cell[e]->getSideNodePos(f, i);
				// Checks posM against all nodes in e2 face.
				for (UInt j = 0; j < c2->nfp; j++) {
					posP = c2->getSideNodePos(f2, j);
					diff = posM - posP;
					// Stores value if the share position.
					if (MathUtils::equal(diff.norm(), (Real) 0.0)) {
						cell[e]->vmapP[f][i] = cell[e]->getSideNode(f2, j);
					}
				}
			}
		}
	}
}
 
void CellGroup::check(const MapGroup& map) const {
	if (cellOffsetId != cell[0]->getId()) {
		throw (Error("ERROR@CellGroup::check"));
	}
	checkIdsAreConsecutive();
	checkNodalMaps(map);
}
 
void CellGroup::checkIdsAreConsecutive() const {
	UInt currentId = cellOffsetId;
	for (UInt i = 1; i < cell.size(); i++) {
		if (cell[i]->getId() == currentId + 1) {
			currentId++;
		} else {
	        throw (Error("ERROR@CellGroup::check"));
		}
	}
}

void CellGroup::checkNodalMaps(const MapGroup& map) const {
	// Checks for vmap.
	CVecR3 diff;
	bool problem = false;
	UInt nK = cell.size();
	for (UInt e = 0; e < nK; e++) {
	   const UInt id = cell[e]->getId();
		for (Int f = 0; f < 4; f++) {
		   const Tet* neigh = map.getNeighbour(id, f);
			const CellTet<ORDER_N>* c2 = getPtrToCell(neigh);
			for (UInt i = 0; i < cell[e]->getNfp(); i++) {
				Int neighNode = cell[e]->vmapP[f][i];
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

