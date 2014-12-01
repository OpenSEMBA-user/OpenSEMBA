#ifndef MAP_H_
	#include "Map.h"
#endif
// =============== Map ========================================================
Map::Map() {

}

Map::~Map() {

}

const Tet*
Map::getInnerTet() const {
	exit(MAP_ERROR);
}

pair<const Tet*, unsigned int>
Map::getInnerFace() const {
	exit(MAP_ERROR);
}

pair<const Tet*, unsigned int>
Map::getOuterFace() const {
	exit(MAP_ERROR);
}

unsigned int
Map::getLocalId() const {
	exit(MAP_ERROR);
}

unsigned int
Map::getVolToF(unsigned int f) const {
	exit(MAP_ERROR);
}

const Tet*
Map::getLocalTet() const {
	exit(MAP_ERROR);
}

const Tet*
Map::getVol(unsigned int f) const {
	exit(MAP_ERROR);
}

bool
Map::isBoundary() const {
	exit(MAP_ERROR);
}

bool
Map::faceIsBoundary(unsigned int face) const {
	exit(MAP_ERROR);
}

void
Map::printInfo() const {
	exit(MAP_ERROR);
}

TriMap::TriMap() {
	local = NULL;
	for (unsigned int i = 0; i < 2; i++) {
		vol[i] = NULL;
		volToF[i] = 0;
	}
}

TriMap::~TriMap() {
	local = NULL;
	for (unsigned int i = 0; i < 2; i++) {
		vol[i] = NULL;
		volToF[i] = 0;
	}
}

TriMap::TriMap(
 const Tri* localSurf,
 pair<const Tet*, const Tet*> neighbours) {
	local = localSurf;
	const Tet* n1 = neighbours.first;
	const Tet* n2 = neighbours.second;
	unsigned int f = n1->getFaceNumber(local);
	if (n1->isLocalFace(f, *local)) {
		vol[0] = n1;
		volToF[0] = f;
		vol[1] = n2;
		volToF[1] = n2->getFaceNumber(local);
	} else {
		vol[0] = n2;
		volToF[0] = n2->getFaceNumber(local);
		vol[1] = n1;
		volToF[1] = f;
	}
}

TriMap&
TriMap::operator=(const TriMap& rhs) {
	if (this == &rhs)
		return *this;
	local = rhs.local;
	for (unsigned int i = 0; i < 2; i++) {
		vol[i] = rhs.vol[i];
		volToF[i] = rhs.volToF[i];
	}
	return *this;
}

pair<const Tet*, unsigned int>
TriMap::getInnerFace() const {
	return pair<const Tet*, unsigned int>(vol[0], volToF[0]);
}

pair<const Tet*, unsigned int>
TriMap::getOuterFace() const {
	return pair<const Tet*, unsigned int>(vol[1], volToF[1]);
}
 
void
TriMap::reassignPointers(const ElementsGroup& nEG) {
	local = nEG.getTriPtrToId(local->getId());
	for (unsigned int i = 0; i < 2; i++) {
		vol[i] = nEG.getTetPtrToId(vol[i]->getId());
	}
}

bool
TriMap::isBoundary() const {
	return (vol[0] == vol[1] && volToF[0] == volToF[1]);
}
 
void
TriMap::printInfo() const {
	cout << "--- TriMap Info ---" << endl;
	cout << "Local Id: " << local->getId() << endl;
	cout << "Neighbours Tet Ids: ";
	for (unsigned int i = 0; i < 2; i++) {
		cout << vol[i]->getId() << " ";
	}
	cout << endl;
	cout << "Through faces: ";
	for (unsigned int i = 0; i < 2; i++) {
		cout << volToF[i] << " ";
	}
	cout << endl;
}

TetMap::TetMap() {
	local = NULL;
	for (unsigned int i = 0; i < 4; i++) {
		vol[i] = NULL;
		volToF[i] = 0;
	}
}

TetMap::~TetMap() {
	local = NULL;
	for (unsigned int i = 0; i < 4; i++) {
		vol[i] = NULL;
		volToF[i] = 0;
	}
}

TetMap::TetMap(
 const Tet* local_,
 const Tet* neighboor[4],
 const unsigned int neighboorFaces[4]) {
	local = local_;
	for (unsigned int i = 0; i < 4; i++) {
		vol[i] = neighboor[i];
		volToF[i] = neighboorFaces[i];
	}
}

const Tet*
TetMap::getLocalTet() const {
	return local;
}

TetMap&
TetMap::operator=(const TetMap& rhs) {
	if (this == &rhs) {
		return *this;
	}
	local = rhs.local;
	for (unsigned int i = 0; i < 4; i++) {
		vol[i] = rhs.vol[i];
		volToF[i] = rhs.volToF[i];
	}
	return *this;
}

void
TetMap::reassignPointers(const ElementsGroup& nEG) {
	local = nEG.getTetPtrToId(local->getId());
	for (unsigned int i = 0; i < 4; i++) {
		vol[i] = nEG.getTetPtrToId(vol[i]->getId());
	}
}

bool
TetMap::faceIsBoundary(unsigned int f) const {
	return (local == vol[f]);
}

void
TetMap::printInfo() const {
	cout << "--- TetMap Info ---" << endl;
	cout << "Local Id: " << local->getId() << endl;
	cout << "Neighbours Ids: ";
	for (unsigned int i = 0; i < local->numberOfFaces(); i++) {
		cout << vol[i]->getId() << " ";
	}
	cout << endl;
	cout << "Through faces: ";
	for (unsigned int i = 0; i < local->numberOfFaces(); i++) {
		cout << volToF[i] << " ";
	}
	cout << endl;
}

MapGroup::MapGroup() {
	offsetIdTri = 0;
	offsetIdTet = 0;
}

MapGroup::~MapGroup() {

}

void
MapGroup::build(const CoordinateGroup& cG, const ElementsGroup& eG) {
	if (eG.tet.size() != 0 && eG.tri.size() != 0) {
		buildMaps(cG, eG);
		offsetIdTet = tet[0].local->getId();
		offsetIdTri = tri[0].local->getId();
		// Checks surface and volume maps.
		check();
	}
}

MapGroup&
MapGroup::operator=(const MapGroup& rhs) {
	if (this == & rhs) {
		return *this;
	}
	tri = rhs.tri;
	tet = rhs.tet;
	offsetIdTri = rhs.offsetIdTri;
	offsetIdTet = rhs.offsetIdTet;
	return *this;
}

const Map*
MapGroup::getPtrToLocalId(const unsigned int id) const {
	if (id < offsetIdTri) {
		return &tet[id - offsetIdTet];
	} else {
		return &tri[id - offsetIdTri];
	}
	exit(MAP_ERROR);
}

void
MapGroup::reassignPointers(const ElementsGroup& newEG) {
#ifndef MAP_DO_NOT_BUILD_SURFACE_MAPS
	for (unsigned int i = 0; i < tri.size(); i++) {
		tri[i].reassignPointers(newEG);
	}
#endif
	for (unsigned int i = 0; i < tet.size(); i++) {
		tet[i].reassignPointers(newEG);
	}
}

void
MapGroup::clear() {
	tri.clear();
	tet.clear();
}

void
MapGroup::buildMaps(
 const CoordinateGroup& cG, const ElementsGroup& elem) {
	// Builds a list with all tetrahedron faces.
	static const unsigned int faces = 4;
	static const unsigned int nVert = 3;
	unsigned int nK = elem.nVolumeElements();
	unsigned int nList = nK * faces;
	DynMatrix<unsigned int> fList(nList, 2 + nVert);
	for (unsigned int k = 0; k < nK; k++) {
		unsigned int id = elem.tet[k]->getId();
		const Tet* aux = elem.getTetPtrToId(id);
		for (unsigned int f = 0; f < faces; f++) {
			unsigned int row = k * faces + f;
			fList(row, 0) = id;
			fList(row, 1) = f;
			unsigned int ordered[nVert];
			aux->getOrderedSideVerticesId(ordered, f);
			for (unsigned int i = 0; i < nVert; i++) {
				fList(row, i + 2) = ordered[i];
			}
		}
	}
	// Sorts according to the coordinates id. Pairing matching faces.
	// If there is not a pair, then that element is not connected with
	// other remaining connected with itself.
	fList.sortRows(2,4);
	// Initializes connectivities matrix.
	DynMatrix<unsigned int> etoe(nK, faces);
	DynMatrix<unsigned int> etof(nK, faces);
	for (unsigned int k = 0; k < nK; k++) {
		for (unsigned int f = 0; f < faces; f++) {
			etoe(k,f) = k;
			etof(k,f) = f;
		}
	}
	// Inserts information of pairings into the connectivity matrix.
	for (unsigned int i = 0; i < (nList-1); i++) {
		bool matches;
		matches = fList(i,2) == fList(i+1,2);
		matches &= fList(i,3) == fList(i+1,3);
		matches &= fList(i,4) == fList(i+1,4);
		if (matches) {
			unsigned int k1 = fList(i,0) - elem.offsetIdTet;
			unsigned int f1 = fList(i,1);
			unsigned int k2 = fList(i+1,0) - elem.offsetIdTet;
			unsigned int f2 = fList(i+1,1);
			etoe(k1,f1) = k2;
			etof(k1,f1) = f2;
			etoe(k2,f2) = k1;
			etof(k2,f2) = f1;
		}
	}
	// Generates tetrahedron maps.
	tet.reserve(nK);
	for (unsigned int k = 0; k < nK; k++) {
		const Tet *local = elem.getTetPtrToId(k + elem.offsetIdTet);
		const Tet *neigh[4];
		unsigned int neighFaces[4];
		for (unsigned int j = 0; j < 4; j++) {
			neigh[j] = elem.getTetPtrToId(etoe(k,j) + elem.offsetIdTet);
			neighFaces[j] = etof(k,j);
		}
		tet.push_back(TetMap(local, neigh, neighFaces));
	}
	// Now uses the generated ordered fList to build the triangle maps.
	unsigned int nS = elem.nSurfaceElements();
	tri.reserve(nS);
	for (unsigned int s = 0; s < nS; s++) {
		unsigned int id = elem.tri[s]->getId();
		const Tri *local = elem.getTriPtrToId(id);
		pair<const Tet*, const Tet*> neigh;
		unsigned int ordered[nVert];
		local->getOrderedVerticesId(ordered);
		unsigned int i = fList.findFirstOcurrenceInColumns(ordered, 2, 3);
		if (i == fList.nRows()) {
			cerr << "Faces list." << endl;
			fList.printInfo();
			cerr << "Surface:" << endl;
			local->printInfo();
			cerr << "ERROR @ building maps" << endl;
			cerr << "A surface has a hanging node." << endl;
		}
		//
		bool matches;
		if (i < nList - 1) {
			matches = fList(i,2) == fList(i+1,2);
			matches &= fList(i,3) == fList(i+1,3);
			matches &= fList(i,4) == fList(i+1,4);
		} else {
			matches = false;
		}
		if (matches) {
			neigh.first = elem.getTetPtrToId(fList(i,0));
			neigh.second = elem.getTetPtrToId(fList(i+1,0));
		} else {
			neigh.first = elem.getTetPtrToId(fList(i,0));
			neigh.second = neigh.first;
		}
		tri.push_back(TriMap(local, neigh));
	}
}
 
#ifdef MAP_USE_OLD_BUILDMAPS
void
MapGroup::buildVolumeMaps(
 const CoordinateGroup& cG,
 const ElementsGroup& elem) {
	// ========================================================================
	// PURPOSE:
	// - This method creates two maps, etoe and etof for the elements.
	// - etoe[pos] stores the id of the elements adjacent to face pos.
	// - etof[pos] stores the face that connects with the element adjacent
	//   through face pos.
	// INPUTS:
	// - ElementTetrahedron_t vector with vertices in their place.
	// OUTPUTS:
	// - Elements end up with their element connectivities in cell[e]->etoe[f].
	// - Number of adjacent face is stored in cell[e]->etof[f].
	// ========================================================================
	// NOTES ON EXECUTION TIMES (rcs.dat - 60k elements): 13/6/2011
	// - Ubuntu 10.04 64b - gcc - dbg ->  0.90 secs
	// - Ubuntu 10.04 64b - gcc - dbg ->  2.04 secs
	// 30/6/2011
	// - Ubuntu 10.04 64b - gcc -  O3 ->  0.50 secs
	// ========================================================================
	unsigned int nK = elem.tet.size();
	unsigned int nV = cG.size();
	unsigned long long *indices;
	unsigned long long  **matchL, **matchR;
	// ----------- Body -------------------------------------------------------
	// Makes a preliminar checking to see if spNodeToNode may be overflowed.
	if (nV > NUMBER_OF_VERTEX_LIMIT) {
		cerr << "ERROR@MapGroup::buildVolumeMaps()" << endl;
		cerr << "Number of vertex can overflow unsigned long long." << endl;
		cerr << "Terminating" << endl;
		exit(MAP_ERROR);
	}
	// Allocates etoe, etof, and ev variables.
	unsigned int **etoe, **etof, **ev; // This will store element vertex.
	ev = new unsigned int*[4];
	for (unsigned int i = 0; i < 4; i++)
		ev[i] = new unsigned int[nK];
	etoe = new unsigned int*[nK];
	etof = new unsigned int*[nK];
	for (unsigned int i = 0; i < nK; i++) {
		etoe[i] = new unsigned int[4];
		etof[i] = new unsigned int[4];
	}
	// Stores nodes ids in ev array.
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < nK; j++)
			ev[i][j] = elem.tet[j]->getVertex(i) - cG.getPtrToId(1);
	// Creates list of all faces 1, then 2, 3 and 4.
	unsigned int tnf = 4*nK;
	unsigned int **temp;
	temp = new unsigned int*[3];
	for (unsigned int i = 0; i < 3; i++)
		temp[i] = new unsigned int[nK];
	int **fNodes;
	fNodes = new int*[tnf];
	for (unsigned int i = 0; i < tnf; i++)
		fNodes[i] = new int [3];
	const static SimplexTet<1> geo;
	for (unsigned int i = 0; i < 4; i++) {
		// Performs: temp = geo1.R[i] * ev;
		for (unsigned int j = 0; j < geo.nfp; j++)
			for (unsigned int k = 0; k < nK; k++)
				for (unsigned int l = 0; l < geo.np; l++)
					if (geo.R[i].val[j][l] == 1)
						temp[j][k] =  ev[l][k];
		// Assigns temp to fnodes
		for (unsigned int j = 0; j < 3; j++) {
			unsigned int iPos = i * nK;
			for (unsigned int k = 0; k < nK; k++)
				fNodes[iPos+k][j] = temp[j][k];
		}
	}
	// Sorts rows in ascending order.
	for (unsigned int i = 0; i < tnf; i++) {
		int fNTemp[3] = {-1, -1, -1};
		for (int j = 2; j >= 0; j--) {
			int maxPos = maxLoc(&fNodes[i][0], 3);
			fNTemp[j] = fNodes[i][maxPos];
			fNodes[i][maxPos] = -1;
		}
		for (unsigned int j = 0; j < 3; j++)
			fNodes[i][j] = fNTemp[j];
	}
	// Initializes default element to element and element
	// to faces connectivity arrays.
	for (unsigned int k = 0; k < nK; k++)
		for (unsigned int j = 0; j < 4; j++) {
			etoe[k][j] = k + 1;
			etof[k][j] = j + 1;
		}
	// Same as before but in string shape.
	int *etoeStr;
	int *etofStr;
	etoeStr = new int[tnf];
	etofStr = new int[tnf];
	// Performs reshape.
	for (unsigned int i = 0; i < tnf; i++) {
		etoeStr[i] = etoe[i % nK][i / nK];
		etofStr[i] = etof[i % nK][i / nK];
	}
	// Uniquely number each set of three faces by their node numbers.
	longArray_s spNodeToNode;
	longArray_s sorted;
	spNodeToNode.val = new unsigned long long*[tnf];
	sorted.val		 = new unsigned long long*[tnf];
	for (unsigned int i = 0; i < tnf; i++) {
		spNodeToNode.val[i]	   = new unsigned long long[4];
		sorted.val[i]		   = new unsigned long long[4];
		spNodeToNode.val[i][0] = (unsigned long long)fNodes[i][0]
		                          * (unsigned long long)nV
				                  * (unsigned long long)nV
								+ (unsigned long long)fNodes[i][1]
								  * (unsigned long long)nV
								+ (unsigned long long)fNodes[i][2]
							    + (unsigned long long)1 ;
		spNodeToNode.val[i][1] = i + 1;
		spNodeToNode.val[i][2] = etoeStr[i];
		spNodeToNode.val[i][3] = etofStr[i];
	}
	// Sorts rows by global face number.
	// Finds global face number maximum.
	arrayQuickSort(spNodeToNode, tnf, 4);
	for (unsigned int i = 0; i < tnf; i++)
		for (unsigned int j = 0; j < 4; j++)
			sorted.val[i][j] = spNodeToNode.val[tnf-i-1][j];
	// Find matches in the sorted face list.
	unsigned int nIndices = 0;
	for (unsigned int i = 0; i < tnf-1; i++)
		if (sorted.val[i][0] == sorted.val[i+1][0])
			nIndices++;
	indices = new unsigned long long[nIndices];
	unsigned int j = 0;
	for (unsigned int i = 0; i < tnf-1; i++)
		if (sorted.val[i][0] == sorted.val[i + 1][0]) {
			indices[j] = i;
			j++;
		}
	// Makes links reflexive.
	// Allocates matchL and matchR arrays containing [...]
	matchL = new unsigned long long*[nIndices * 2];
	matchR = new unsigned long long*[nIndices * 2];
	for (unsigned int i = 0; i < (2*nIndices); i++) {
		matchL[i] = new unsigned long long[4];
		matchR[i] = new unsigned long long[4];
	}
	// Stores sorted indices.
	for (unsigned int i = 0; i < nIndices; i++)
		for (j = 0; j < 4; j++) {
			matchL[i][j] = sorted.val[indices[i]][j];
			matchR[i][j] = sorted.val[indices[i]+1][j];
		}
	for (unsigned int i = 0; i < nIndices; i++)
		for (j=0; j < 4; j++) {
			matchL[i+nIndices][j] = sorted.val[indices[i]+1][j];
			matchR[i+nIndices][j] = sorted.val[indices[i]][j];
		}
	// Inserts matches.
	for (unsigned int i = 0; i < (2*nIndices); i++) {
		etoeStr[matchL[i][1] - 1] = matchR[i][2];
		etofStr[matchL[i][1] - 1] = matchR[i][3];
	}
	// Creates arrays.
	int nNodes = 0;
	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int k = 0; k < nK; k++) {
			etoe[k][i] = etoeStr[nNodes + k];
			etof[k][i] = etofStr[nNodes + k];
		}
		nNodes += nK;
	}
	// Info in result.
	tet.reserve(nK);
	const Tet *local;
	const Tet *neighboor[4];
	unsigned int neighboorFaces[4];
	for (unsigned int k = 0; k < nK; k++) {
		local = elem.tet[k];
		for (unsigned int j = 0; j < 4; j++) {
			neighboor[j] = elem.tet[etoe[k][j] - 1];
			neighboorFaces[j] = etof[k][j] - 1;
		}
		tet.push_back(TetMap(local, neighboor, neighboorFaces));
	}
	// Frees allocated memory.
	delete [] indices;
	for (unsigned int i = 0; i<nIndices*2; i++) {
 		delete [] matchL[i];
		delete [] matchR[i];
	}
	delete [] matchL;
	delete [] matchR;
	delete [] etoeStr;
	delete [] etofStr;
	for (unsigned int i = 0; i < tnf; i++) {
		delete [] spNodeToNode.val[i];
		delete [] sorted.val[i];
		delete [] fNodes[i];
	}
	delete [] spNodeToNode.val;
	delete [] sorted.val;
	delete [] fNodes;
	for (register int i = 0; i < 4; i++)
		delete [] ev[i];
	delete [] ev;
	for (register unsigned int i = 0; i < nK; i++) {
		delete [] etoe[i];
		delete [] etof[i];
	}
	delete [] etoe;
	delete [] etof;
	for (unsigned int i = 0; i < 3; i++)
		delete [] temp[i];
	delete [] temp;
}
#endif
#ifdef MAP_USE_OLD_BUILDMAPS
void
MapGroup::buildSurfaceMaps(const ElementsGroup& eG) {
	tri.resize(eG.tri.size(), TriMap());
#ifndef MAP_DO_NOT_BUILD_SURFACE_MAPS
	unsigned int i;
#ifdef MAP_USE_OPENMP
	#pragma omp parallel for private(i)
#endif
	for (i = 0; i < eG.tri.size(); i++) {
		const Tri* auxTri = eG.tri[i];
		pair<const Tet*, const Tet*> neighbours = getNeighbours(auxTri);
		tri[i] = (TriMap(auxTri, neighbours));
	}
#endif
}
#endif
 
pair<const Tet*, const Tet*>
MapGroup::getNeighbours(const Tri* param) const {
	// PURPOSE:
	// Returns a pointers to Tets that is adjacent to the SurfaceElement.
	bool isNeighbour = false;
	unsigned int vPresent;
	unsigned int nK = tet.size();
	for (unsigned int i = 0; i < nK; i++) {
		const Tet* cTet = tet[i].local;
		for (unsigned int f = 0; f < 4; f++) {
			// Checks current face.
			vPresent = 0;
			for (unsigned int j = 0; j < 3; j++) {
				for (unsigned int k = 0; k < 3; k++) {
					if (*param->getVertex(j) == *cTet->getSideVertex(f, k)) {
						vPresent++;
					}
				}
				if (vPresent == 3) {
					isNeighbour = true;
				}
			}
			// If all nodes are in present face.
			if (isNeighbour) {
				const Tet* cTet2 = tet[i].vol[f];
				pair<const Tet*, const Tet*> res(cTet, cTet2);
				return res;
			}
		}
	}
	//
	cerr << "ERROR @ Mesh::getNeighbours(...)" << endl;
	param->printInfo();
	cerr << " Tri " << param->getId() << " is disconnected.";
	exit(MAP_ERROR);
}
 
void
MapGroup::checkReciprocity() const {
	bool problem = false;
	unsigned int count = 0;
	for (unsigned int e = 0; e < tet.size(); e++) {
		const Tet* e1 = tet[e].local;
		for (unsigned int f = 0; f < 4; f++) {
			const Tet* e2 = tet[e].vol[f];
			unsigned int f2 = tet[e].volToF[f];
			const Map* m2 = getPtrToLocalId(e2->getId());
			if (m2->getVol(f2) != e1) {
				if (!problem) {
					cerr << "ERROR@MapGroup::check()"<< endl;
					cerr << "Map fail reciprocity check:"    << endl;
				}
				cerr << "Elem " << e1->getId() << ", face " << f << endl;
				cerr << "connects with element "
				 << e2->getId() << " face " << f2 << endl;
				cerr << "But that face connects with element "
			     << m2->getVol(f2)->getId() << endl;
				count++;
				if (count == 10) {
					cerr << "And maybe more..." << endl;
					cerr << "Terminating" << endl;
					exit(MAP_ERROR);
				}
				problem = true;
			}
		}
	}
}
 
void
MapGroup::check() const {
	checkReciprocity();
	checkDisconnectedTetrahedrons();
}

const Map*
MapGroup::getPtrToNeighMap(
 const Map* inner,
 unsigned int f) const {
	unsigned int neighId = inner->getVol(f)->getId();
	return getPtrToLocalId(neighId);
}

void
MapGroup::checkDisconnectedTetrahedrons() const {
	// Checks if exist disconnected elements according to its etoe.
	unsigned int nK = tet.size();
	bool firstFound = false;
	for (unsigned int e = 0; e < nK; e++) {
		bool disconnectedElement = true;
		const Tet* local = tet[e].local;
		for (unsigned int f = 0; f < 4; f++)
			disconnectedElement &= (tet[e].vol[f] == local);
		if (disconnectedElement && !firstFound) {
			cerr << "ERROR@MapGroup::checkDisconectedTetrahedrons()" << endl;
			firstFound = true;
		}
		if (disconnectedElement) {
			cerr << "Disconnected element #" << local->getId() << endl;
		}
	}
	if (firstFound) {
		exit(MAP_ERROR);
	}
}

