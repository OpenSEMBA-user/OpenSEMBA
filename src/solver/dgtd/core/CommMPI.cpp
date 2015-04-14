/*
 * CommunicationsMPI.cpp
 *
 *  Created on: Apr 17, 2013
 *      Author: luis
 */

#include "CommMPI.h"

CommMPI::CommMPI() {
	world = MPI_COMM_WORLD;
	int rc = MPI_Init(NULL, NULL);
	if (rc != MPI_SUCCESS) {
		cout << "Error starting MPI program. Terminating." << endl;
		MPI_Abort(world, rc);
    }
	MPI_Comm_size(world,&nTasks);
	MPI_Comm_rank(world,&task);
	commitFieldStruct();
	pSize = NULL;
	pOffset = NULL;
	nPartitions = 1;
	nNeighId = NULL;
	neighId = NULL;
	neighFSize = NULL;
	neighFOffset = NULL;
	nDofSize = NULL;
	nDofOffset = NULL;
	nTasksInHost = countTasksInLocalHost();
	len = 0;
}

CommMPI::~CommMPI() {
	MPI_Finalize();
}

int
CommMPI::getNumberOfTasks() const {
	return nTasks;
}

int
CommMPI::getTask() const {
	return task;
}

bool
CommMPI::isMaster() const {
	if (task == master) {
		return true;
	}
	return false;
}

void
CommMPI::gatherFieldsMaster(
 double* Ex, double* Ey, double* Ez,
 double* Hx, double* Hy, double* Hz,
 const double* lEx, const double* lEy, const double* lEz,
 const double* lHx, const double* lHy, const double* lHz) const {
	assert(isMaster());
	// Packs fields in field struct.
	uint lFieldSize = getLocalFieldSize();
	Field_s *lField;
	lField = new Field_s[lFieldSize];
	packFields(lField, lEx,lEy,lEz,lHx,lHy,lHz, lFieldSize);
	// Allocates space for general field.
	uint gFieldSize = getGlobalSize() * np;
	Field_s *gField;
	gField = new Field_s[gFieldSize];
	// Gathers fields in master.
	MPI_Barrier(world);
	MPI_Gatherv(lField, lFieldSize, MPIField, gField,
	 getFieldSizes(), getFieldOffsets(), MPIField, master, world);
	delete lField;
	// Unpacks in general fields variables.
	unpackFields(Ex,Ey,Ez,Hx,Hy,Hz,gField, gFieldSize);
	delete gField;
}

void
CommMPI::gatherFieldsSlave(
 const double* lEx, const double* lEy, const double* lEz,
 const double* lHx, const double* lHy, const double* lHz) const {
	assert(!isMaster());
	// Packs fields in field struct.
	uint lFSize = getLocalFieldSize();
	Field_s * lField;
	lField = new Field_s[lFSize];
	packFields(lField, lEx,lEy,lEz,lHx,lHy,lHz, lFSize);
	// Gathers fields in master.
	MPI_Barrier(world);
	MPI_Gatherv(lField, lFSize, MPIField, NULL,
	 NULL, NULL, NULL, master, world);
	delete lField;
}

void
CommMPI::setPartitionSizes(
 const vector<vector<uint> >& partId) {
	// Sets partition sizes and offsets.
	pSize = new int[nTasks];
	pOffset = new int [nTasks];
	fSize = new int [nTasks];
	fOffset = new int [nTasks];
	for (int i = 0; i < nTasks; i++) {
		pSize[i] = partId[i].size();
		fSize[i] = partId[i].size() * np;
		if (i == 0) {
			pOffset[i] = 0;
			fOffset[i] = 0;
		} else {
			pOffset[i] = pSize[i-1] + pOffset[i-1];
			fOffset[i] = fSize[i-1] + fOffset[i-1];
		}
	}
	setLocalSizeAndOffset(pSize[task], pOffset[task]);
}

uint
CommMPI::getLocalOffset() const {
	assert(pOffset != NULL);
	return pOffset[task];
}

uint
CommMPI::getLocalSize() const {
	assert(pSize != NULL);
	return pSize[task];
}

void
CommMPI::syncNeighbourFields(
 double* nEx, double* nEy, double* nEz,
 double* nHx, double* nHy, double* nHz,
 const double* Ex, const double* Ey, const double* Ez,
 const double* Hx, const double* Hy,	const double* Hz) const {
	for (int t = 0; t < nTasks; t++) {
		double *sField;
		sField = new double[nDofSize[t][task]];
		// Packs fields for task t.
		if (t != task) {
			uint fInd = 0;
			for (int i = 0; i < nNeighId[t]; i++) {
				uint id = neighId[t][i];
				if (isLocalId(id)) {
					uint fp = getRelPosOfId(id) * np;
					for (uint j = 0; j < np; j++) {
						sField[fInd++] = Ex[fp];
						sField[fInd++] = Ey[fp];
						sField[fInd++] = Ez[fp];
						sField[fInd++] = Hx[fp];
						sField[fInd++] = Hy[fp];
						sField[fInd++] = Hz[fp];
						fp++;
					}
				}
			}
		}
		// Gathers fields.
		uint fSize = nNeighId[t] * np;
		double *rField;
		rField = new double[fSize*6];
		MPI_Gatherv(sField, nDofSize[t][task], MPI_DOUBLE,
		 rField, nDofSize[t], nDofOffset[t], MPI_DOUBLE, t, world);
		// Unpacks fields if this process is the receiver.
		if (task == t) {
			uint nn = (uint) nNeighId[t];
			for (uint i = 0; i < nn; i++) {
				uint fInd = i * np * 6;
				uint frp = neighElemRP[i] * np;
				assert((uint) neighElemRP[i*np] < nn);
				for (uint j = 0; j < np; j++) {
					assert(0 <= frp && frp < nn * np);
					nEx[frp] = rField[fInd++];
					nEy[frp] = rField[fInd++];
					nEz[frp] = rField[fInd++];
					nHx[frp] = rField[fInd++];
					nHy[frp] = rField[fInd++];
					nHz[frp] = rField[fInd++];
					frp++;
				}
			}
		}
		// Frees all memory.
		delete sField;
		delete rField;
	}
}

void
CommMPI::initNeighbourFields(const vector<uint>& nIds) {
	assert(nNeighId == NULL);
	assert(neighId == NULL);
	// Gathers number of neighbours.
	nNeighId = new int[nTasks];
	uint nNeigh = nIds.size();
	MPI_Allgather(&nNeigh, 1, MPI_UNSIGNED,
     nNeighId, 1, MPI_UNSIGNED, world);
	// Gathers neighbour Ids.
	// Gathers data.
	uint totalNumberOfNeigh = 0;
	for (int t = 0; t < nTasks; t++) {
		totalNumberOfNeigh += nNeighId[t];
	}
	int auxOffset[nTasks];
	auxOffset[0] = 0;
	for (int t = 1; t < nTasks; t++) {
		auxOffset[t] = nNeighId[t-1] + auxOffset[t-1];
	}
	int neighIdGlobal[totalNumberOfNeigh]; // Aux. receiver buffer.
	int neighIdsBuffer[nNeigh];
	for (uint i = 0; i < nNeigh; i++) {
		neighIdsBuffer[i] = nIds[i];
	}
	MPI_Allgatherv(neighIdsBuffer, nIds.size(), MPI_UNSIGNED,
	 neighIdGlobal, nNeighId, auxOffset, MPI_UNSIGNED, world);
	// Unpacks buffer.
	neighId = new uint*[nTasks];
	for (int t = 0; t < nTasks; t++) {
		neighId[t] = new uint[nNeighId[t]];
		for (int i = 0; i < nNeighId[t]; i++) {
			neighId[t][i] = neighIdGlobal[auxOffset[t] + i];
		}
	}
	// Stores task relational data. Sizes and offsets.
	assert(neighFSize == NULL);
	assert(neighFOffset == NULL);
	neighFSize = new int *[nTasks];
	neighFOffset = new int *[nTasks];
	nDofSize = new int *[nTasks];
	nDofOffset = new int *[nTasks];
	for (int i = 0; i < nTasks; i++) {
		neighFSize[i] = new int[nTasks];
		neighFOffset[i] = new int[nTasks];
		nDofSize[i] = new int[nTasks];
		nDofOffset[i] = new int[nTasks];
	}
	for (int t = 0; t < nTasks; t++) {
		// Counts Ids coming from each task.
		for (int i = 0; i < nTasks; i++) {
			neighFSize[t][i] = 0;
			nDofSize[t][i] = 0;
		}
		for (int i = 0; i < nNeighId[t]; i++) {
			int idTask = getTaskOfId(neighId[t][i]);
			neighFSize[t][idTask] += np;
			nDofSize[t][idTask] += np * 6;
		}
		// Inits offsets.
		for (int i = 0; i < nTasks; i++) {
			if (i == 0) {
				neighFOffset[t][0] = 0;
				nDofOffset[t][0] = 0;
			} else {
				neighFOffset[t][i] =
				 neighFOffset[t][i-1] + neighFSize[t][i-1];
				nDofOffset[t][i] =
				 nDofOffset[t][i-1] + nDofSize[t][i-1];
			}
		}
	}
	// Sends relative positions of neigh nodes in local node.
	int **neighSize, **neighOffset;
	neighSize = new int*[nTasks];
	neighOffset = new int*[nTasks];
	for (int t = 0; t < nTasks; t++) {
		neighSize[t] = new int[nTasks];
		neighOffset[t] = new int[nTasks];
		// Counts Ids coming from each task.
		for (int i = 0; i < nTasks; i++) {
			neighSize[t][i] = neighFSize[t][i] / np;
			neighOffset[t][i] = neighFOffset[t][i] / np;
		}
	}
	// Sends relative positions.
	for (int t = 0; t < nTasks; t++) {
		uint *srp;
		uint nFSize = neighFSize[t][task];
		srp = new uint[nFSize];
		if (t != task) {
			uint k = 0;
			for (int i = 0; i < nNeighId[t]; i++) {
				uint id = neighId[t][i];
				if (isLocalId(id)) {
					srp[k++] = i;
				}
			}
		} else {
			neighElemRP = new uint[nNeighId[t]];
		}
		MPI_Gatherv(srp, neighSize[t][task], MPI_UNSIGNED,
		 neighElemRP, neighSize[t], neighOffset[t], MPI_UNSIGNED,
		 t, world);
		if (t != task) {
			delete srp;
		}
	}
	// Frees memory.
	delete neighOffset;
	delete neighSize;
	// Performs several checkings (only when DBG)
	if (nTasks == 2) {
		assert(checkNNeighCoherence(nNeighId));
	}
	assert(checkVectorsAreEqual(nNeighId[task], neighId[task], nIds));
	assert(checkNeighFSizes());
}

void
CommMPI::printInfo() const {
	cout << " --- CommMPI info --- " << endl;
	printf ("Number of tasks= %d My rank= %d\n", nTasks, task);
	cout << " Local Size: " << getLocalSize() << endl;
	cout << " Global Size: " << getGlobalSize() << endl;
	if (pSize != NULL) {
		cout << "pSize: ";
		for (int t = 0; t < nTasks; t++) {
			cout << pSize[t] << " ";
		}
		cout << endl;
	}
	if (pOffset != NULL) {
		cout << "pOffset: ";
		for (int t = 0; t < nTasks; t++) {
			cout << pOffset[t] << " ";
		}
		cout << endl;
	}
	if (nNeighId != NULL) {
		cout << "nneighId: " << endl;
		for (int t = 0; t < nTasks; t++) {
			cout << "task #" << t << ": " << nNeighId[t] << endl;
		}
	}
	if (neighId != NULL) {
		cout << "neighId" << endl;
		for (int t = 0; t < nTasks; t++) {
			cout << "task #" << t << ": ";
			for (int i = 0; i < nNeighId[t]; i++) {
				cout << neighId[t][i] << " ";
			}
			cout << endl;
		}
	}
	if (neighFSize != NULL) {
		cout << "neighFSize" << endl;
		for (int t = 0; t < nTasks; t++) {
			for (int s = 0; s < nTasks; s++) {
				cout << neighFSize[t][s] << " ";
			}
			cout << endl;
		}
		cout << "neighFOffset" << endl;
		for (int t = 0; t < nTasks; t++) {
			for (int s = 0; s < nTasks; s++) {
				cout << neighFOffset[t][s] << " ";
			}
			cout << endl;
		}
		cout << "nDofSize" << endl;
		for (int t = 0; t < nTasks; t++) {
			for (int s = 0; s < nTasks; s++) {
				cout << nDofSize[t][s] << " ";
			}
			cout << endl;
		}
		cout << "nDofOffset" << endl;
		for (int t = 0; t < nTasks; t++) {
			for (int s = 0; s < nTasks; s++) {
				cout << nDofOffset[t][s] << " ";
			}
			cout << endl;
		}
	}
	printOrderingInfo();
	cout << " --- End of CommMPI info ---" << endl;
}

void
CommMPI::packFields(
 Field *field,
 const double *Ex, const double *Ey, const double *Ez,
 const double *Hx, const double *Hy, const double *Hz,
 const uint fSize) const {
	for (uint i = 0; i < fSize; i++) {
		field[i].Ex = Ex[i];
		field[i].Ey = Ey[i];
		field[i].Ez = Ez[i];
		field[i].Hx = Hx[i];
		field[i].Hy = Hy[i];
		field[i].Hz = Hz[i];
	}
}

void
CommMPI::unpackFields(
 double *Ex, double *Ey, double *Ez,
 double *Hx, double *Hy, double *Hz,
 const Field *field, const uint fSize) const {
	for (uint i = 0; i < fSize; i++) {
		Ex[i] = field[i].Ex;
		Ey[i] = field[i].Ey;
		Ez[i] = field[i].Ez;
		Hx[i] = field[i].Hx;
		Hy[i] = field[i].Hy;
		Hz[i] = field[i].Hz;
	}
}

void
CommMPI::commitFieldStruct() {
	const int count = 7;
	int lengths[count] = {1, 1, 1, 1, 1, 1, 1};
	int iTS, dTS;
	MPI_Type_size(MPI_UNSIGNED, &iTS);
	MPI_Type_size(MPI_DOUBLE, &dTS);
	MPI_Aint disp[count] = { 0, iTS, iTS+dTS, iTS+2*dTS,
	 iTS+3*dTS, iTS+4*dTS, iTS+5*dTS};
	MPI_Datatype types[count] = {
	 MPI_UNSIGNED, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE,
	 MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
	MPI_Type_create_struct(count,lengths,disp,types, &MPIField);
	MPI_Type_commit(&MPIField);
}

uint
CommMPI::getLocalFieldSize() const {
	assert(fSize != NULL);
	return fSize[task];
}

int*
CommMPI::getFieldOffsets() const{
	assert(fOffset != NULL);
	return fOffset;
}

int*
CommMPI::getFieldSizes() const{
	assert(fSize != NULL);
	return fSize;
}


int
CommMPI::getTaskOfId(const uint id) const {
	uint rp = getGlobalRelPosOfId(id);
	for (int t = 0; t < (nTasks - 1); t++) {
		if (rp < (uint) pOffset[t+1]) {
			return t;
		}
	}
	return (nTasks - 1);
}

bool
CommMPI::checkNNeighCoherence(int* nneigh) const {
	assert(nneigh != NULL);
	return (nneigh[0] == nneigh[1]);
}

bool
CommMPI::checkVectorsAreEqual(
 const uint vSize,
 const uint* v,
 const vector<uint>& nIds) const {
	assert(vSize == nIds.size());
	bool ok = true;
	for (uint i = 0; i < vSize; i++) {
		if (v[i] != nIds[i]) {
			cout << v[i] << " is not equal to " << nIds[i] << endl;
			ok = false;
		}
	}
	if (!ok) {
		cout << "ERROR@checkVectorsAreEqual @ Task: " << task << endl;
		cout << "Above values of gathered neighbour ids are not equal"
		 << endl;
	}
	return ok;
}

double
CommMPI::reduceToGlobalMinimum(double val) const {
	double res;
	static const int count = 1;
	MPI_Allreduce(&val, &res, count,
	 MPI_DOUBLE, MPI_MIN, world);
	return res;
}

int
CommMPI::countTasksInLocalHost() const {
	// Counts number of tasks running on this host.
	char localHostName[MPI_MAX_PROCESSOR_NAME];
	int localHostNameLen;
	MPI_Get_processor_name(localHostName, &localHostNameLen);
	char hostName[nTasks][MPI_MAX_PROCESSOR_NAME];
	int hostNameLen[nTasks];
	MPI_Allgather(
	 &localHostNameLen, 1, MPI_INT, hostNameLen, 1, MPI_INT,
		world);
	MPI_Allgather(
	 localHostName, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, hostName,
	 MPI_MAX_PROCESSOR_NAME, MPI_CHAR, world);
	int res = 0;
	for (int t = 0; t < nTasks; t++) {
		bool isLocalHost = true;
		if (localHostNameLen != hostNameLen[t]) {
			isLocalHost &= false;
		}
		for (int i = 0; i < localHostNameLen; i++) {
			isLocalHost &= (localHostName[i] == hostName[t][i]);
		}
		if (isLocalHost) {
			res++;
		}
	}
	assert(res > 0);
	return res;
}

int
CommMPI::getNumOfTasksOnThisHost() const {
	return nTasksInHost;
}

bool
CommMPI::checkNeighFSizes() const {
	bool sizesOk = true;
	for (int t = 0; t < nTasks; t++) {
		uint neighFSum = 0;
		for (int s = 0; s < nTasks; s++) {
			neighFSum += neighFSize[t][s];
		}
		sizesOk &= (neighFSum == nNeighId[t] * np);
	}
	if (!sizesOk) {
		cerr << endl << "neighFSizes are inconsistent with nNeighId" << endl;
	}
	bool diagOk = true;
	for (int t = 0; t < nTasks; t++) {
			diagOk &= (neighFSize[t][t] == 0);
	}
	if (!diagOk) {
		printInfo();
		cerr << endl << "neighFSizes has non-zero diag." << endl;
		cerr << endl << "A task is neighbouring itself." << endl;
	}
	bool symmetryOk = true;
	DynMatrix<int> aux(nTasks, nTasks, neighFSize);
	symmetryOk = aux.isSymmetric();
	if (!symmetryOk) {
		printInfo();
		cerr << endl << "ERROR @ checkNeighSizes()" << endl;
		cerr << endl << "neighFSizes is not symmetric." << endl;
	}
	return (sizesOk && diagOk && symmetryOk);
}

vector<uint>
CommMPI::getThreadsOfTasks() const {
	uint *taskThreads;
	taskThreads = new uint[nTasks];
#ifdef USE_OPENMP
	uint localThreads = omp_get_max_threads();
#else
	uint localThreads = 1;
#endif
	MPI_Allgather(&localThreads, 1, MPI_UNSIGNED,
     taskThreads, 1, MPI_UNSIGNED, world);
	vector<uint> res;
	res.reserve(nTasks);
	for (int t = 0; t < nTasks; t++) {
		res.push_back(taskThreads[t]);
	}
	delete taskThreads;
	return res;
}
