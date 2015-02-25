#ifndef MESHVOLUME_H_
	#include "MeshVolume.h"
#endif

MeshVolume::MeshVolume() {
	grid_ = NULL;
}

MeshVolume::MeshVolume(
 const CoordinateGroup& vIn,
 const ElementsGroup& elementIn) : Mesh(vIn, elementIn, NULL) {
	grid_ = NULL;
	//
    check();
}

MeshVolume::MeshVolume(
 const CoordinateGroup& vIn,
 const ElementsGroup& elementIn,
 const Grid3* grid) : Mesh(vIn, elementIn, grid) {
    check();
}
 
MeshVolume::~MeshVolume() {
	if (grid_ != NULL) {
		delete grid_;
	}
}
 
MeshVolume&
MeshVolume::operator=(const MeshVolume& param) {
	if (this == &param) {
		return *this;
	}
	cG_ = param.cG_;
	elem_ = param.elem_;
	elem_.reassignPointers(cG_);
	map = param.map;
	map.reassignPointers(elem_);
	if (param.grid_ != NULL) {
		grid_ = new Grid3(*param.grid_);
	} else {
		grid_ = NULL;
	}
	return *this;
}

pair<const Volume*, unsigned int>
MeshVolume::getBoundary(const Surface* surf) const {
	return map.getInnerFace(surf->getId());
}
 
vector<pair<const Volume*, unsigned int> >
MeshVolume::getInternalBorder(
 const vector<unsigned int>& region) const {
	// Runs over all elements contained in the region vector detecting
	// the internal border. Returns a vector containing the element
	// faces composing the internal border.
	if (elem_.areTetrahedrons(region)) {
		return getInternalBorderOfTetRegion(region);
	} else if (elem_.areTriangles(region)) {
		return getInternalBorderOfTriRegion(region);
	}
	cout << "ERROR @ getInternalBorder" << endl;
	cout << "Unable to detect region." << endl;
	exit(-1);
}

 
vector<pair<const Volume*, unsigned int> >
MeshVolume::getExternalBorder(
 const vector<unsigned int>& elemIds) const {
	// Generates a vector of pairs pointers to tetrahedrons and faces that
	// connects with the region specified by the elemIds inputted.
	// If the element's face in the inputted region is in the computational
	// border nothing is returned.
	vector<pair<const Volume*, unsigned int> > internal;
	internal = getInternalBorder(elemIds);
	unsigned int nI = internal.size();
	vector<pair<const Volume*, unsigned int> > external;
	external.reserve(nI);
	for (unsigned int i = 0; i < nI; i++) {
		unsigned int inId = internal[i].first->getId();
		unsigned int inFace = internal[i].second;
		const Tet* outVol = map.getNeighbour(inId, inFace);
		unsigned int outFace = map.getVolToF(inId, inFace);
		if (outVol->getId() != inId || inFace != outFace)  {
			pair<const Volume*, unsigned int> aux(outVol, outFace);
			external.push_back(aux);
		}
	}
	return external;
}
 
vector<pair<const Tet*, unsigned int> >
MeshVolume::getBorderWithNormal(
 const vector<pair<const Tet*, unsigned int> >& border,
 const CartesianVector<double, 3>& normal) {
	const unsigned int nK = border.size();
	vector<pair<const Tet*, unsigned int> > res;
	res.reserve(nK);
	for (unsigned int i = 0; i < nK; i++) {
		const Tet* tet = border[i].first;
		const unsigned int face = border[i].second;
		CartesianVector<double,3> tetNormal = tet->sideNormal(face);
		if (tetNormal == normal && !tet->isCurvedFace(face)) {
			res.push_back(border[i]);
		}
	}
	return res;
}

vector<unsigned int>
MeshVolume::getAdjacentElements(const vector<unsigned int>& region) const {
	vector<pair<const Volume*, unsigned int> > outer;
	outer = getExternalBorder(region);
	unsigned int nOut = outer.size();
	// Removes repeated.
	DynMatrix<unsigned int> aux(nOut,1);
	for (unsigned int i = 0; i < nOut; i++) {
		aux(i,0) = outer[i].first->getId();
	}
	aux.sortAndRemoveRepeatedRows_omp();
	// Prepares result.
	vector<unsigned int> res(aux.nRows(), 0);
	for (unsigned int i = 0; i < aux.nRows(); i++) {
		res[i] = aux(i,0);
	}
	return res;
}
 
void
MeshVolume::printInfo() const {
	Mesh::printInfo();
	elem_.printInfo();
	if (grid_ != NULL) {
		grid_->printInfo();
	}
}

vector<vector<unsigned int> >
MeshVolume::getPartitionsIds(const unsigned int nDivisions) const {
	// Calls weight balanced partitioner with a zero size weight vector
	vector<pair<unsigned int,int> > idWgt;
	return getPartitionsIds(nDivisions, idWgt);
}

vector<vector<unsigned int> >
MeshVolume::getPartitionsIds(
 const unsigned int nDivisions,
 const vector<pair<unsigned int,int> > idWgt) const {
	return getPartitionsIds(nDivisions, idWgt, NULL);
}
 
vector<vector<unsigned int> >
MeshVolume::getPartitionsIds(
 const unsigned int nDivisions,
 const vector<pair<unsigned int,int> > idWgt,
 const double* taskPower) const {
	// Metis v5 manual:
	// [...] take as input the element-node array of the mesh and
	// compute a k-way partitioning for both its elements and its nodes
	// idWgt contains id and weight pairs.
	vector<vector<unsigned int> > res;
	res.resize(nDivisions);
	// Accounts for the one partition case.
	if (nDivisions == 1) {
		unsigned int nK = elem_.nVolumeElements();
		res[0].resize(nK, 0);
		for (unsigned int i = 0; i < nK; i++) {
			res[0][i] = elem_.tet[i]->getId();
		}
		return res;
	}
#ifdef MESH_ALLOW_PARTITIONING
	// Prepares mesh info.
	cout << " - Preparing mesh info... " << flush;
	idx_t ne = elem_.nVolumeElements();
	idx_t *eptr, *eind;
	eptr = new idx_t[ne+1];
	eind = new idx_t[ne*4];
	unsigned int counter = 0;
	eptr[0] = counter;
	for (int i = 0; i < (int) ne; i++) {
		const Tet* vol = elem_.tet[i];
		for (unsigned int j = 0; j < vol->numberOfVertices(); j++) {
			eind[counter++] = vol->getVertex(j)->id - 1;
		}
		eptr[i+1] = counter;
	}
	cout << "OK" << endl;
	// Relabels ids, needed by quadratic or linearized meshes.
	cout << " - Relabeling... " << flush;
	DynMatrix<int> id(ne*4,3);
	for (int i = 0; i < ne*4; i++) {
		id(i,0) = i;
		id(i,1) = eind[i];
		id(i,2) = 0;
	}
	id.sortRows_omp(1,1);
	int label = 0;
	for (int i = 1; i < ne*4; i++) {
		if (id(i,1) == id(i-1,1)) {
			id(i,2) = label;
		} else {
			id(i,2) = ++label;
		}
	}
	id.sortRows_omp(0,0);
	for (int i = 0; i < ne*4; i++) {
		eind[i] = id(i,2);
	}
	idx_t nn = label+1; // Number of vertices.
	cout << "OK" << endl;
	// Copies weights.
	cout << " - Copying weights... " << flush;
 	idx_t *vwgt;
	if (idWgt.size() == 0) {
		vwgt = NULL;
	} else {
		vwgt = new idx_t[ne];
		for (int e = 0; e < ne; e++) {
			vwgt[e] = idWgt[e].second;
		}
	}
	idx_t *vsize = NULL;
	idx_t nparts = nDivisions;
	idx_t objval;
	idx_t *epart;
	epart = new idx_t[ne];
	idx_t *npart;
	npart = new idx_t[nn];
	cout << "OK" << endl;
	// Computes task computational powers.
	real_t *tpwgts = NULL;
	if (taskPower != NULL) {
		tpwgts = new real_t[nDivisions];
		real_t sum = 0.0;
		for (unsigned int i = 0; i < nDivisions; i++) {
			tpwgts[i] = taskPower[i];
			sum += tpwgts[i];
		}
		assert(abs(sum) - 1.0e-16 < 1.0);
	}
	// METIS options.
	cout << " - Setting Options... " << flush;
	idx_t options[METIS_NOPTIONS];
	int status;
	status = METIS_SetDefaultOptions(options);
	options[METIS_OPTION_PTYPE] = METIS_PTYPE_KWAY;
	options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_CUT;
	options[METIS_OPTION_SEED] = (idx_t) 0;
//	options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_VOL;
	// c numbering. Starts from 0.
	options[METIS_OPTION_NUMBERING] = 0;
	cout << "OK" << endl;
	// Calls METIS partition function for meshes.
	idx_t ncommon = 3; // Number of common vertices per element.
	cout << " - Calling Part Mesh Dual... " << flush;
	status = METIS_PartMeshDual(
  	 &ne, &nn, eptr, eind, vwgt, vsize, &ncommon, &nparts,
	 tpwgts, options, &objval, epart, npart);
	if (status != METIS_OK) {
		cerr << "ERROR@Mesh::getPartitionIds()" << endl;
		cerr << "METIS_PartMeshDual fn failed with error: "
		 << status << endl;
		exit(MESH_ERROR);
	}
	cout << "OK" << endl;
	// Converts result.
	for (unsigned int i = 0; i < nDivisions; i++) {
		res[i].reserve(ne);
	}
	for (int i = 0; i < ne; i++) {
		unsigned int id = elem_.tet[i]->getId();
		res[epart[i]].push_back(id);
	}
	// Frees memory.
	delete vwgt;
	delete epart;
	delete npart;
	delete eptr;
	delete eind;
	// Returns result.
	return res;
#else
	cerr << "ERROR @ Mesh::getPartition(): "
	 << "Mesh partitioning is not allowed." << endl;
	return vector<vector<uint> >();
#endif
}

vector<unsigned int>
MeshVolume::getIdsOfCurvedTets() const {
	vector<unsigned int> res;
	unsigned int nK = elem_.nVolumeElements();
	for (unsigned int k = 0; k < nK; k++) {
		if (elem_.tet[k]->isCurved()) {
			res.push_back(elem_.tet[k]->getId());
		}
	}
	return res;
}



vector<Tri3>
MeshVolume::getTriWithMatId(
        const uint matId,
        const bool ignoreTet) const {
	vector<Tri3> res;
	const uint nTri = elem_.tri.size();
	const uint nTet = elem_.tet.size();
	res.reserve(nTri + nTet);
	// --- Runs over surfaces -------------------------------------------------
	const uint nTri3 = elem_.tri3.size();
	for (uint i = 0; i < nTri3; i++) {
		if (elem_.tri3[i].getMatId() == matId) {
			res.push_back(elem_.tri3[i]);
		}
	}
	const uint nTri6 = elem_.tri6.size();
	for (uint i = 0; i < nTri6; i++) {
		if (elem_.tri6[i].getMatId() == matId) {
			res.push_back(elem_.tri6[i].linearize());
		}
	}
	// --- Runs over tetrahedrons ---------------------------------------------
	if (!ignoreTet) {
	    vector<uint> tetIds;
	    tetIds.reserve(nTet);
	    for (uint i = 0; i < nTet; i++) {
	        // Generates list of tetrahedrons ids.
	        if (elem_.tet[i]->getMatId() == matId) {
	            tetIds.push_back(elem_.tet[i]->getId());
	        }
	    }
	    // Gets internal border of tetrahedron volume.
	    vector<pair<const Volume*, unsigned int> > internalBorder;
	    internalBorder = getInternalBorder(tetIds);
	    // Converts internal border to Tri3.
	    for (uint i = 0; i < internalBorder.size(); i++) {
	        const Volume* vol = internalBorder[i].first;
	        const Tet* tet = dynamic_cast<const Tet*>(vol);
	        const uint face = internalBorder[i].second;
	        res.push_back(tet->getTri3Face(face));
	    }
	}
	return res;
}

vector<Tri3>
MeshVolume::getTriWithId(const vector<uint>& ids) const {
	vector<Tri3> res;
	res.reserve(elem_.tri3.size());
	for (uint i = 0; i < ids.size(); i++) {
		for (uint j = 0; j < elem_.tri3.size(); j++) {
			if (ids[i] == elem_.tri3[j].getId()) {
				res.push_back(elem_.tri3[j]);
			}
		}
	}
	return res;
}

vector<unsigned int>
MeshVolume::getTetIds(
 const vector<unsigned int> elemIds) const {
	vector<unsigned int> res;
	res.reserve(elemIds.size());
	const unsigned int nId = elemIds.size();
	for (unsigned int i = 0; i < nId; i++) {
		if (elem_.getPtrToId(elemIds[i])->isTet()) {
			res.push_back(elemIds[i]);
		}
	}
	return res;
}

bool
MeshVolume::isFloatingCoordinate(const CoordD3* param) const {
	for (uint i = 0; i < elem_.element.size(); i++) {
		for (uint j = 0; j < elem_.element[i]->numberOfCoordinates(); j++) {
			if (*param == *elem_.element[i]->getV(j)) {
				return false;
			}
		}
	}
	return true;
}

vector<BoxD3>
MeshVolume::getRectilinearHexesInsideRegion(
        const vector<const Element*>& region) const {
    // Determines positions to query.
    vector<uint> ids = elem_.getIds(region);
    BoxD3 bound(getBound(ids));
    vector<CVecD3> center = grid_->getCenterOfNaturalCellsInside(bound);
    // Determines if positions are inside tetrahedrons.
    vector<BoxD3> res;
    res.reserve(center.size());
    for (uint i = 0; i < center.size(); i++) {
        for (uint j = 0; j < region.size(); j++) {
            if (region[j]->isInnerPoint(center[i])) {
                res.push_back(grid_->getBoundingBoxContaining(center[i]));
                break;
            }
        }
    }
    return res;
}

void
MeshVolume::check() const {
//	if (elem.nVolumeElements() == 0) {
//		cerr << "ERROR @ Mesh::check()" << endl;
//		cerr << "The mesh does not contain volumees." << endl;
//		exit(MESH_ERROR);
//	}
//	if (elem.tri.size() == 0) {
//		cerr << "ERROR @ Mesh::check()" << endl;
//		cerr << "The mesh does not contain surfaces." << endl;
//		exit(MESH_ERROR);
//	}
//	if (map.tet.size() != elem.tet.size()
//	 || map.tri.size() != elem.tri.size()) {
//		cerr << "ERROR@Mesh::check()" << endl;
//		cerr << "map has a different size than its elements." << endl;
//		cerr << "#Tet maps: " << map.tet.size() << endl;
//		cerr << "#Tri maps: " << map.tri.size() << endl;
//		cerr << "#Tets: " << elem.nVolumeElements() << endl;
//		cerr << "#Tris: " << elem.nSurfaceElements() << endl;
//		exit(MESH_ERROR);
//	}
//	checkAreaCoherence();
	// TODO: MeshVolume::check for overlapping surfaces.
}

vector<pair<const Volume*, unsigned int> >
MeshVolume::getInternalBorderOfTetRegion(
 const vector<unsigned int>& region) const {
#ifndef MESH_USE_OLD_ALGORITHM_TO_GETINTERNAL_BORDER
	// Builds a list with all tetrahedron faces.
	static const unsigned int faces = 4;
	static const unsigned int nVert = 3;
	unsigned int nK = region.size();
	unsigned int nList = nK * faces;
	DynMatrix<unsigned int> fList(nList, 2 + nVert);
	for (unsigned int k = 0; k < nK; k++) {
		const Tet* tet = elem_.getTetPtrToId(region[k]);
		for (unsigned int f = 0; f < faces; f++) {
			unsigned int row = k * faces + f;
			fList(row, 0) = tet->getId();
			fList(row, 1) = f;
			unsigned int ordered[nVert];
			tet->getOrderedSideVerticesId(ordered, f);
			for (unsigned int i = 0; i < nVert; i++) {
				fList(row, i + 2) = ordered[i];
			}
		}
	}
	// Sorts according to the coordinates id. Pairing matching faces. ---------
	// If there is not a pair, then that element is not connected with other
	// remaining connected with itself.
	fList.sortRows_omp(2,4);
	// Copies non repeated faces into result vector.
	vector<pair<const Volume*, unsigned int> > res;
	res.reserve(nK);
	bool matches;
	for (unsigned int k = 0; k < nList; k++) {
		if (k < nList-1) {
			matches = fList(k, 2) == fList(k+1, 2);
			matches &= fList(k, 3) == fList(k+1, 3);
			matches &= fList(k, 4) == fList(k+1, 4);
		} else {
			matches = false;
		}
		if (matches) {
			k++;
		} else {
			const Tet* tet = elem_.getTetPtrToId(fList(k,0));
			unsigned int face = fList(k,1);
			pair<const Tet*, unsigned int> aux(tet, face);
			res.push_back(aux);
		}
	}
	return res;
#else
	unsigned int regionSize = region.size();
	unsigned int nK = elem_.tet.size();
	unsigned int offset = elem_.offsetIdTet;
	unsigned int nBoundaries = 0;
	//
	DynMatrix<long> adjElem(nK, 4);
	for (unsigned int j = 0; j < regionSize; j++) {
		unsigned int id = region[j];
		unsigned int e = id - offset;
		const Map* tetMap = map.getPtrToLocalId(id);
		// Initializes adjElem.
		for (unsigned int f = 0; f < 4; f++) {
			adjElem(e,f) = tetMap->getVol(f)->getId();
		}
		// Runs over all adjElem and if any element there doesn't belong to
		// tFiExc it is substituted by a zero.
		for (unsigned int f = 0; f < 4; f++) {
			bool found = false;
			for (unsigned int l = 0; l < regionSize; l++)
				if (region[l] == tetMap->getVol(f)->getId()) {
					found = true;
					break;
				}
			if (!found)
				adjElem(e, f) = -1;
			// If detects that the adjElem belongs to a boundary of the
			// total field region, the adds 1 to the counter.
			if ((adjElem(e,f) == long(id)) || (adjElem(e, f) == -1))
				nBoundaries++;
		}
	} // Ends computation of adjElem.
	// Writes boundary. corresponding to inner face of the region.
	vector<pair<const Tet*, unsigned int> > res;
	res.reserve(nBoundaries);
	for (unsigned int j = 0; j < regionSize; j++) {
		unsigned int id = region[j];
		unsigned int e = id - offset;
		for (unsigned int f = 0; f < 4; f++)
			if ((adjElem(e,f) == long(id)) || (adjElem(e,f) == -1)) {
				const Tet* auxTet = elem_.getTetPtrToId(id);
				pair<const Tet*, unsigned int> bc(auxTet, f);
				res.push_back(bc);
			}
	}
	//
	return res;
#endif
}

vector<pair<const Volume*, unsigned int> >
MeshVolume::getInternalBorderOfTriRegion(
 const vector<unsigned int>& region) const {
	unsigned int nE = region.size();
	vector<pair<const Volume*, unsigned int> > res(nE);
	for (unsigned int i = 0; i < nE; i++) {
	   const Surface* surf =
	    dynamic_cast<const Surface*>(elem_.getPtrToId(region[i]));
		res[i] = getBoundary(surf);
	}
	return res;
}

bool
MeshVolume::checkAreaCoherence() const {
	unsigned int nElem = elem_.nVolumeElements();
	bool isOk = true;
	for (unsigned int e = 0; e < nElem; e++) {
		unsigned int id = elem_.tet[e]->getId();
		const Tet *local = elem_.getTetPtrToId(id);
		for (unsigned int f = 0; f < local->numberOfFaces(); f++) {
			pair<const Volume*, unsigned int> localFace(local, f);
			pair<const Volume*, unsigned int> neighFace;
			neighFace = getNeighConnection(localFace);
			const Volume* neigh = neighFace.first;
			unsigned int f2 = neighFace.second;
			double diff =
			 local->getAreaOfFace(f) - neigh->getAreaOfFace(f2);
			if (abs(diff) > areaDiffTolerance) {
				cerr << "Id: " << local->getId()
				 << " Face: " << f
				 << " Diff: " << diff << endl;
				isOk = false;
			}
		}
	}
	if (!isOk) {
		cerr << "ERROR @ checkAreaCoherence()" << endl;
		cerr << "Check above id-face pairs." << endl;
	}
	return isOk;
}

bool
MeshVolume::isOnBoundary(const CVecD3 pos) const {
#warning "Not implemented"
    return false;
}

CVecD3 MeshVolume::getClosestVertex(const CVecD3 pos) const {
#warning "Not implemented"
    return CVecD3();
}

vector<const Polygon*> MeshVolume::getMaterialBoundary(
        const uint matId,
        const uint layId) const {
#warning "Not implemented"
    return vector<const Polygon*>();
}

vector<BoxD3> MeshVolume::discretizeWithinBoundary(
        const uint matId,
        const uint layId) const {
#warning "Not implemented"
    return vector<BoxD3>();
}
