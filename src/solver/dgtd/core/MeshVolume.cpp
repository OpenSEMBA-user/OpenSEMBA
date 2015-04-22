#ifndef MESHVOLUME_H_
	#include "MeshVolume.h"
#endif

MeshVolume::MeshVolume() {
	grid_ = NULL;
}

MeshVolume::MeshVolume(
 const GroupCoordinates& vIn,
 const GroupElements& elementIn) : Mesh(vIn, elementIn, NULL) {
	grid_ = NULL;
}

MeshVolume::MeshVolume(
 const GroupCoordinates& vIn,
 const GroupElements& elementIn,
 const Grid3* grid) : Mesh(vIn, elementIn, grid) {
}
 
MeshVolume::~MeshVolume() {
	if (grid_ != NULL) {
		delete grid_;
	}
}


void
MeshVolume::createAndAssignPML(
      const PMVolumePML::Direction direction[3],
      const vector<pair<const Volume*, unsigned int> >& internalBorder,
      MeshVolume* mesh) {
   // Computes bound of PML pointing in this direction.
   BoxD3 bound = mesh->getBound(internalBorder);
   pair<CVecD3,CVecD3> pmlBound;
   for (unsigned int i = 0; i < 3; i++) {
      if (direction[i] == PMVolumePML::minus) {
         pmlBound.second(i) = bound.getMin()(i);
         pmlBound.first(i) = - numeric_limits<double>::infinity() ;
      } else if (direction[i] == PMVolumePML::plus) {
         pmlBound.first(i) = bound.getMax()(i);
         pmlBound.first(i) = numeric_limits<double>::infinity() ;
      } else {
         pmlBound.first(i) = bound.getMin()(i);
         pmlBound.second(i) = bound.getMax()(i);
      }
   }
   // Gets ids of PMLs inside the pml bound. Removes non-tet.
   vector<unsigned int> allPMLIds = mesh->getIdsInsideBound(pmlBound);
   vector<unsigned int> PMLIds = mesh->getTetIds(allPMLIds);
   // Creates new PML material for that bound and sets in mesh.
   if (PMLIds.size() != 0) {
      unsigned int lastId = count();
      pml_.push_back(new PMVolumePML(lastId+1, direction, mesh->getBound(PMLIds)));
      updatePointers();
      mesh->setMaterialIds(PMLIds, lastId+1);
   }
}


void
MeshVolume::detectAndAssignPMLRegions() {
   if (countPML() == 0) {
      return;
   }
   const unsigned int pmlId = getPML()->getId();
   vector<unsigned int> notPMLId = mesh->elem_.getIdsWithoutMaterialId(pmlId);
   vector<unsigned int> internalId = mesh->getTetIds(notPMLId);
   vector<pair<const Volume*, unsigned int> > internalBorder =
         mesh->getInternalBorder(internalId);
   // Creates PML material stretched towards +x.
   PMVolumePML::Direction direction[3];
   for (unsigned int i = 0; i < PMVolumePML::possibleDirections; i++) {
      getDirection(direction, i);
      createAndAssignPML(direction, internalBorder, mesh);
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
		cerr << endl << "ERROR@Mesh::getPartitionIds()" << endl;
		cerr << endl << "METIS_PartMeshDual fn failed with error: "
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
	cerr << endl << "ERROR @ Mesh::getPartition(): "
	 << "Mesh partitioning is not allowed." << endl;
	return vector<vector<uint> >();
#endif
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
				cerr << endl << "Id: " << local->getId()
				 << " Face: " << f
				 << " Diff: " << diff << endl;
				isOk = false;
			}
		}
	}
	if (!isOk) {
		cerr << endl << "ERROR @ checkAreaCoherence()" << endl;
		cerr << endl << "Check above id-face pairs." << endl;
	}
	return isOk;
}


