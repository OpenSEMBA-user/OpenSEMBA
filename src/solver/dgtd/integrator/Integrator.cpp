/*
 * Integrator.cpp
 *
 *  Created on: Feb 21, 2013
 *      Author: luis
 */

#include "Integrator.h"

Integrator::Integrator() {
	mindt = 0.0;
	nTiers = 0;
	tierRange = NULL;
	doLTS = true;
	growSmallerTiers = 0;
	maxNumOfTiers = 0;
	timeStepSize = 0.0;
	solver = NULL;
	partIds.clear();
}

Integrator::~Integrator() {
	for (uint i = 0; i < nTiers; i++) {
		delete tierRange[i];
	}
}

void
Integrator::setSolver(DG* solver_) {
	solver = solver_;
}

uint
Integrator::getNTiers() const {
	return nTiers;
}

double
Integrator::getMaxDT() const {
	if (doLTS) {
		return (mindt / pow(getMaxTimeRatio(), double(nTiers-1)));
	} else {
		return mindt;
	}
}

double
Integrator::getMinDT() const {
	return mindt;
}

vector<vector<uint> >
Integrator::getTiersIds() const {
	vector<vector<uint> > res;
	for (uint tier = 0; tier < nTiers; tier++) {
		vector<uint> tierIds = getIdsOfTier(tier);
		res.push_back(tierIds);
	}
	return res;
}

vector<vector<uint> >
Integrator::getStagesIds() const {
	vector<vector<uint> > res;
	for (uint stage = 0; stage < getNStages(); stage++) {
		vector<uint> stageIds =	getIdsOfStage(stage);
		res.push_back(stageIds);
	}
	return res;
}

vector<vector<uint> >
Integrator::getPartitionsIds() const {
	//assert(partIds.size() != 0);
	return partIds;
}

vector<pair<uint, int> >
Integrator::getComputationalWeights(const MeshVolume* msh) const {
	const int curlFlops = 1; // TODO curlFlops are not considered.
	const int fluxFlops = 0; // TODO fluxFlops are not considered.
	int flops = curlFlops + fluxFlops;
	uint nK = msh->nVolumeElements();
	vector<pair<uint,int> > idWgt;
	idWgt.reserve(nK);
	for (uint e = 0; e < nK; e++) {
		pair<uint,int> aux;
		uint id = getIdOfGlobalRelPos(e);
		aux.first = id;
		aux.second =
		 getNumOfIterationsPerBigTimeStep(e) * flops;
		idWgt.push_back(aux);
	}
	return idWgt;
}

pair<uint, uint>
Integrator::getRange(const uint tier, const uint stage) const {
	assert(tier < nTiers);
	assert(stage < getNStages());
	assert(tierRange != NULL);
	return tierRange[tier][stage];
}

uint
Integrator::getNPartitions() const {
	return partIds.size();
}

void
Integrator::partitionate(
 const MeshVolume* msh,
 Comm* comm) {
	cout << " - Getting computational weights... " << flush;
	vector<pair<uint,int> > idWgt;
	idWgt = getComputationalWeights(msh);
	cout << "OK" << endl;
	vector<vector <uint> > partId;
	cout << " - Obtaining partition ids... " << flush;
	partId =
	 msh->getPartitionsIds(comm->getNumberOfTasks(), idWgt);
	cout << "OK" << endl;
	cout << " - Setting partition sizes... " << flush;
	comm->setPartitionSizes(partId);
	cout << "OK" << endl;
	cout << " - Reordering partitions... " << flush;
	reorder(partId, comm->getLocalOffset(), comm->getLocalSize());
	cout << "OK" << endl;
}

void
Integrator::printInfo() const {
	cout << "--- SolverInfo ---" << endl;
	cout << "Min. time step: " << mindt*1E12 << " [ps]" << endl;
	cout << "Max. time step: " << getMaxDT()*1E12 << " [ps]" << endl;
	cout << "Number of tiers: " << nTiers << endl;
	if (nTiers > 1) {
		for (uint i = 0; i < nTiers; i++) {
			cout << "# of Cells in tier " << i << ": "
			 << getNumberOfCellsInTier(i) << endl;
			for (uint j = 0; j < getNStages(); j++) {
				cout << "--> Range stage #" << j << ": "
				 << getRange(i,j).first << ", "
				 << getRange(i,j).second << endl;
			}
		}
	}
}

void
Integrator::init(
 const MeshVolume& mesh,
 const PhysicalModelGroup& pmGroup,
 const ArgumentsCudg3d* arg) {
	growSmallerTiers = arg->getGrowSmallerTiers();
	maxNumOfTiers = arg->getMaxNumberOfTiers();
	doLTS = !arg->isNoLTS();
	if (!doLTS) {
		maxNumOfTiers = 1;
	}
	cout << "- Building tier info... " << flush;
	buildTierInfo(mesh, pmGroup);
	cout << " OK" << endl;
	cout << "- Building Relative Positions of Ids... " << flush;
	buildRelPosOfIds(timeTierList);
	cout << " OK" << endl;
	cout << "- Building Tier Range... " << flush;
	tierRange = buildTierRange(tierRange, timeTierList);
	cout <<  "OK" << endl;
	//
	assert(timeStepSize != 0.0);
	assert(tierRange != NULL);
}

double
Integrator::getMaxTimeStep(
 const Tet* tet,
 const PhysicalModel* mat) const {
	// Returns the maximum time step allowed for this cell.
	double fS1 = 0.0;
	for (uint f = 0; f < tet->numberOfFaces(); f++) {
		double area = tet->getAreaOfFace(f);
		double volume = tet->getVolume();
		double fS2 = area / volume;
		if (fS2 > fS1) {
			fS1 = fS2;
		}
	}
	double dt = cfl *  (1.0 / double(fS1 * ORDER_N * ORDER_N));
	// Checks case of electrical dispersive materials.
	checkMaterialStabilityForDT(mat, dt);
	return (dt * timeStepSize);
}

uint
Integrator::getNumberOfCellsInTier(const uint tier) const {
	assert(tier < nTiers);
	assert(tierRange != NULL);
	uint first = getRange(tier, 0).first;
	uint last = getRange(tier, getNStages() - 1).second;
	uint res = last - first;
	return res;
}

vector<uint>
Integrator::getIdsOfTier(uint tier) const {
	assert(tier < nTiers);
	uint nK = timeTierList.nRows();
	vector<uint> res;
	res.reserve(nK);
	for (uint i = 0; i < nK; i++) {
		if (timeTierList(i,1) == tier) {
			res.push_back(timeTierList(i,0));
		}
	}
	return res;
}

vector<uint>
Integrator::getIdsOfStage(uint stage) const {
	assert(stage < getNStages());
	uint nK = timeTierList.nRows();
	vector<uint> res;
	res.reserve(nK);
	for (uint i = 0; i < nK; i++) {
		if (timeTierList(i,2) == stage) {
			res.push_back(timeTierList(i,0));
		}
	}
	return res;
}

void
Integrator::reorder(
 const vector<vector<uint> >& partitionId,
 const uint localOffset,
 const uint localSize) {
	partIds = partitionId;
	// Reorders timeTierList according to partitions.
	reorderTimeTierList(partitionId);
	// Now aux stores the final ordering considering partitions.
	buildRelPosOfIds(timeTierList);
	// Builds time tier range
	DynMatrix<uint> reducedList(localSize,3);
	for (uint i = 0; i < localSize; i++) {
		for (uint j = 0; j < 3; j++) {
			reducedList(i,j) = timeTierList(i + localOffset, j);
		}
	}
	tierRange = buildTierRange(tierRange, reducedList);
}

void
Integrator::reorderTimeTierList(
 const vector<vector<uint> >& partitionId) {
	uint nK = timeTierList.nRows();
	DynMatrix<uint> aux(nK, 5); // relPos - Ids - Part - Tier - Stage
	for (uint k = 0; k < nK; k++) {
		aux(k, 0) = k;
		aux(k, 1) = timeTierList(k, 0);
		aux(k, 2) = 0; // Temporary assignation of partition.
		aux(k, 3) = timeTierList(k, 1); // Tier assignation.
		aux(k, 4) = timeTierList(k, 2);
	}
	aux.sortRows_omp(1, 1);
	uint initId = aux(0, 1);
	uint nParts = partitionId.size();
	for (uint p = 0; p < nParts; p++) {
		for (uint i = 0; i < partitionId[p].size(); i++) {
			uint id = partitionId[p][i];
			aux(id - initId, 2) = p;
		}
	}
	aux.sortRows_omp(2, 4);
	for (uint i = 0; i < nK; i++) {
		timeTierList(i, 0) = aux(i, 1);
		timeTierList(i, 1) = aux(i, 3);
		timeTierList(i, 2) = aux(i, 4);
	}
}

void
Integrator::buildTierInfo(
 const MeshVolume& mesh,
 const PhysicalModelGroup& pmGroup) {
	assignTiersBasedOnMaxTimeStep(mesh, pmGroup);
	// Grows smallest tier regions for smoothing.
	if (nTiers > 1 && growSmallerTiers > 0) {
		growSmallestTierRegions(growSmallerTiers, mesh);
	}
	if (nTiers > 1) {
		assignStages(mesh);
	}
	// Ensures that all elem within same tier/stage are consecutive.
	if (nTiers > 1) {
		timeTierList.sortRows_omp(1,2);
		nTiers = timeTierList.maxValInCol(1) + 1;
	}
}

pair<uint,uint>**
Integrator::buildTierRange(
 pair<uint,uint> **range,
 const DynMatrix<uint>& list) {
	uint nK = list.nRows();
	static const uint vS = 2;
	const uint nStages = getNStages();
	// Allocates memory for tierRange.
	if (range != NULL) {
		for (uint i = 0; i < nTiers; i++) {
			delete [] range[i];
		}
		delete [] range;
	}
	range = new pair<uint,uint>*[nTiers];
	for (uint i = 0; i < nTiers; i++) {
		range[i] = new pair<uint,uint>[nStages];
	}
	// Assigns ranges for tier 0.
	if (nTiers == 1) {
		for (uint i = 0; i < nStages; i++) {
			range[0][i].first = 0;
			range[0][i].second = nK;
		}
	} else {
		static const uint nextKey[2] = {1, 0};
		uint e2 = list.findFirstOcurrenceInColumns(nextKey,1,vS);
		for (uint i = 0; i < nStages; i++) {
			range[0][i].first = 0;
			range[0][i].second = e2;
		}
	}
	//
	uint key[2], nextKey[2];
	for (uint tier = 1; tier < nTiers; tier++) {
		for (uint stage = 0; stage < nStages; stage++) {
			key[0] = tier;
			key[1] = stage;
			uint e1 = list.findFirstOcurrenceInColumns(key,1,vS);
			uint e2;
			if (tier+1 != nTiers || stage+1 != nStages) {
				nextKey[0] = tier + (stage+1) / nStages;
				nextKey[1] = (stage+1) % nStages;
				e2 = list.findFirstOcurrenceInColumns(nextKey,1,vS);
			} else {
				e2 = nK;
			}
			range[tier][stage].first = e1;
			range[tier][stage].second = e2;
		}
	}
	return range;
}

void
Integrator::checkMaterialStabilityForDT(
 const PhysicalModel* mat,
 const double dt) const {
   const PMVolumeDispersive* disp = dynamic_cast<const PMVolumeDispersive*>(mat);
   if (disp != NULL) {
	   for (uint p = 0; p < disp->getPoleNumber(); p++) {
			double polePeriod = 1.0 / abs(disp->getPole(p));
			if (polePeriod < dt) {
				cerr << endl << "ERROR@Integrator: "
				 << " Contains pole #" << p + 1
				 << " with value " << disp->getPole(p)
				 << " will cause an unstability for dt"
				 << dt << endl;
				mat->printInfo();
			}
		}
	}
}

void
Integrator::growSmallestTierRegions(
 const uint toGrow,
 const MeshVolume& mesh) {
	uint nK = mesh.elem_.nVolumeElements();
	timeTierList.sortRows_omp(0,0);
	for (uint tier = 0; tier < nTiers-1; tier++) {
		// Creates a list with all the elements belonging to this tier.
		vector<uint> elem;
		elem.reserve(nK);
		bool isInRegion;
		for (uint k = 0; k < nK; k++) {
			isInRegion = (timeTierList(k,1) == tier);
			if (isInRegion) {
				elem.push_back(timeTierList(k,0));
			}
		}
		vector<uint> grownElem, newNeigh, neigh;
		grownElem = elem;
		for (uint stage = 0; stage < toGrow; stage++) {
			newNeigh = mesh.getAdjacentElements(grownElem);
			neigh.insert(
			 neigh.end(), newNeigh.begin(), newNeigh.end());
			grownElem.insert(
			 grownElem.end(), newNeigh.begin(), newNeigh.end());
		}
		for (uint k = 0; k < neigh.size(); k++) {
			uint id = neigh[k];
			uint row =
			timeTierList.findFirstOcurrenceInColumns(&id,0,1);
			timeTierList(row,1) = tier;
		}
	}
	// Updates number of tiers.
	timeTierList.sortRows_omp(1,1);
	nTiers = timeTierList(nK-1,1) + 1;
}

void
Integrator::assignTiersBasedOnMaxTimeStep(
 const MeshVolume& mesh,
 const PhysicalModelGroup& pmGroup) {
	uint nK = mesh.elem_.nVolumeElements();
	DynMatrix<double> dtList(nK, 4);
	mindt = 0.0;
	for (uint k = 0; k < nK; k++) {
		const Tet* tet = mesh.elem_.tet[k];
		const PMVolume* mat =
		 pmGroup.getPMVolumeWithId(tet->getMatId());
		double dt = getMaxTimeStep(tet, mat);
		if (mindt > dt || mindt == 0.0) {
			mindt = dt;
		}
		dtList(k,0) = tet->getId();
		dtList(k,1) = dt;
		dtList(k,2) = noTier;
		dtList(k,3) = getNStages() - 1;
	}
	if (maxNumOfTiers != 1) {
		dtList.sortRows_omp(1,1);
		double maxdtList = dtList(nK-1,1);
		double ratio = getMaxTimeRatio();
		nTiers = floor(log(mindt/maxdtList)/log(ratio)) + 1;
		if (maxNumOfTiers > 0 && nTiers > maxNumOfTiers) {
			nTiers = maxNumOfTiers;
		}
		for (uint tier = 0; tier < nTiers; tier++) {
			double inf = mindt / pow(ratio, double(tier));
			double sup;
			if (tier+1 == maxNumOfTiers) {
				sup = numeric_limits<double>::max();
			} else {
				sup = mindt / pow(ratio, double(tier + 1));
			}
			for (uint k = 0; k < nK; k++) {
				if (dtList(k,1) >= inf && dtList(k,1) < sup) {
					dtList(k,2) = tier;
				}
			}
		}
	} else {
		nTiers = 1;
	}
	timeTierList.copy(dtList.eliminateColumns(1,1));
}

void
Integrator::assignStages(const MeshVolume& mesh) {
	if (nTiers == 1) {
		return;
	}
	// ----------- Reassigns tiers ------------------------------------
	uint nK = mesh.elem_.nVolumeElements();
	timeTierList.sortRows_omp(0,0);
	const uint nStages = getNStages();
	for (uint tier = 0; tier < nTiers-2; tier++) {
		// Creates a list with all the elements belonging to this tier.
		vector<uint> elem;
		elem.reserve(nK);
		bool isInRegion;
		for (uint k = 0; k < nK; k++) {
			isInRegion = (timeTierList(k,1) == tier);
			if (isInRegion) {
				elem.push_back(timeTierList(k,0));
			}
		}
		vector<uint> grownElem, newNeigh, neigh;
		grownElem = elem;
		for (uint stage = 0; stage < (nStages * growStages); stage++) {
			newNeigh = mesh.getAdjacentElements(grownElem);
			neigh.insert(
			 neigh.end(), newNeigh.begin(), newNeigh.end());
			grownElem.insert(
			 grownElem.end(), newNeigh.begin(), newNeigh.end());
		}
		for (uint k = 0; k < neigh.size(); k++) {
			uint id = neigh[k];
			uint row =
			 timeTierList.findFirstOcurrenceInColumns(&id,0,1);
			if (timeTierList(row,1) > tier + 1) {
				timeTierList(row,1) = tier + 1;
			}
		}
	}
	// ----------- Assigns stages -------------------------------------
	for (uint tier = 0; tier < nTiers-1; tier++) {
		vector<uint> elem;
		elem.reserve(nK);
		bool isInRegion;
		for (uint k = 0; k < nK; k++) {
			isInRegion = (timeTierList(k,1) == tier);
			if (isInRegion) {
				elem.push_back(timeTierList(k,0));
			}
		}
		vector<uint> grownElem, newNeigh, neigh;
		grownElem = elem;
		for (uint stage = 0; stage < nStages; stage++) {
			for (uint times = 0; times < growStages; times++) {
				newNeigh = mesh.getAdjacentElements(grownElem);
				grownElem.insert(
				 grownElem.end(), newNeigh.begin(), newNeigh.end());
				for (uint k = 0; k < newNeigh.size(); k++) {
					uint id = newNeigh[k];
					uint row =
					 timeTierList.findFirstOcurrenceInColumns(&id,0,1);
					if (row < nK) {
						if (timeTierList(row,1) > tier) {
							timeTierList(row,2) = stage;
						}
					}
				}
			}
		}
	}
}

vector<pair<uint, uint> >
Integrator::getIdPartitionVector(
 const vector<vector<uint> >& pId) const {
	assert(pId.size() != 0);
	vector<pair<uint,uint> > res;
	uint nPart = pId.size();
	uint gSize = 0;
	for (uint p = 0; p < nPart; p++) {
		gSize += pId[p].size();
	}
	res.reserve(gSize);
	pair<uint,uint> idPart;
	for (uint p = 0; p < nPart; p++) {
		for (uint k = 0; k < pId[p].size(); k++) {
			idPart.first = pId[p][k];
			idPart.second = p;
			res.push_back(idPart);
		}
	}
	return res;
}

//void
//Integrator::processStopRequest() {
//	// Terminates the program storing the fields if key is pressed.
//	nonblock(NB_ENABLE);
//	int endingRequest = 0;
//	endingRequest = kbhit();
//	if (endingRequest != 0 && fgetc(stdin)=='q') {
//		cout << "Key 'q' was pressed, terminating." << endl;
//		// Writes fields for resume.
//		cout << " - Writing resume files." << endl;
//		const double *fPtrs[6];
//		getFieldPtrs(fPtrs);
//		out->writeResumeFile(fPtrs, tStep, nK);
//		cout << " - Program was succesfully terminated." << endl;
//		exit(EXIT_SUCCESS);
//	}
//	nonblock(NB_DISABLE);
//}

//int
//Integrator::kbhit()	{
//	struct timeval tv;
//	fd_set fds;
//	tv.tv_sec = 0;
//	tv.tv_usec = 0;
//	FD_ZERO(&fds);
//	FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
//	select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
//	return FD_ISSET(STDIN_FILENO, &fds);
//}

//void
//Integrator::nonblock(int state) {
//	struct termios ttystate;
//	//get the terminal state
//	tcgetattr(STDIN_FILENO, &ttystate);
//	if (state == NB_ENABLE) {
//		//turn off canonical mode
//		ttystate.c_lflag &= ~ICANON;
//		//minimum of number input read.
//		ttystate.c_cc[VMIN] = 1;
//	}
//	else if (state == NB_DISABLE) {
//		//turn on canonical mode
//		ttystate.c_lflag |= ICANON;
//	}
//	//set the terminal attributes.
//	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
//}
