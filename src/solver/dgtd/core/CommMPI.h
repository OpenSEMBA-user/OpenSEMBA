/*
 * CommunicationsMPI.h
 *
 *  Created on: Apr 17, 2013
 *      Author: luis
 */

#ifndef COMMUNICATIONSMPI_H_
#define COMMUNICATIONSMPI_H_

#include "Comm.h"
#include <vector>
#include <mpi.h>

//#define USE_OLD_SYNC

#ifndef MESH_ALLOW_PARTITIONING
	#error "MPI communications need a mesh partitioning method."
#endif

#ifndef ORDER_N
	#error "ORDER_N is undefined"
#endif

struct Field_s {
	int rp;
	double Ex, Ey, Ez, Hx, Hy, Hz;
};

class CommMPI : public Comm {
public:
	CommMPI();
	virtual ~CommMPI();
	int
	 getNumberOfTasks() const;
	int
	 getTask() const;
	uint
	 getLocalOffset() const;
	uint
	 getLocalSize() const;
	int
	 getNumOfTasksOnThisHost() const;
	void
	 setPartitionSizes(
	  const vector<vector<uint> >& partId);
	void
	 gatherFieldsMaster(
 	  double* Ex, double* Ey, double* Ez,
	  double* Hx, double* Hy, double* Hz,
	  const double* lEx, const double* lEy, const double* lEz,
	  const double* lHx, const double* lHy, const double* lHz) const;
	void
	 gatherFieldsSlave(
 	  const double* Ex, const double* Ey, const double* Ez,
	  const double* Hx, const double* Hy, const double* Hz) const;
	bool
	 isMaster() const;
	void
	 syncNeighbourFields(
	  double* nEx, double* nEy, double* nEz,
	  double* nHx, double* nHy, double* nHz,
	  const double* Ex, const double* Ey, const double* Ez,
	  const double* Hx, const double* Hy, const double* Hz) const;
	void
	 initNeighbourFields(const vector<uint>& nIds);
	double
	 reduceToGlobalMinimum(double val) const;
	void
	 printInfo() const;
private:
	static const int master = 0;
	static const uint np = ((ORDER_N+1)*(ORDER_N+2)*(ORDER_N+3)/6);
	static const bool weightPartitions = true;
	int nTasks;
	int nTasksInHost;
	int task;
	int len;
	int* pSize;
	int* pOffset;
	int* fSize;
	int* fOffset;
	uint* neighElemRP;
	MPI_Comm world;
	MPI_Datatype MPIField;
	uint nPartitions;
	int* nNeighId;
	uint** neighId;
	int **neighFSize;
	int **neighFOffset;
	int **nDofSize;
	int **nDofOffset;
	int*
	 getDofSizes() const;
	int*
	 getDofOffsets() const;
	uint
	 getLocalDofSize() const;
	uint
	 getGlobalDofSize() const;
	void
	 packFields(
	  Field_s *field,
	  const double *Ex, const double *Ey, const double *Ez,
	  const double *Hx, const double *Hy, const double *Hz,
	  const uint fSize) const;
	void
	 unpackFields(
	  double *Ex, double *Ey, double *Ez,
	  double *Hx, double *Hy, double *Hz,
	  const Field_s *field, const uint fSize) const;
	void
	 commitFieldStruct();
	int*
	 getFieldSizes() const;
	int*
	 getFieldOffsets() const;
	uint
	 getLocalFieldSize() const;
	int
	 getTaskOfId(const uint id) const;
	bool
	 checkNNeighCoherence(int* nneigh) const;
	bool
	 checkVectorsAreEqual(
	  const uint vSize,
	  const uint* v,
	  const vector<uint>& nIds) const;
	bool
	 checkNeighFSizes() const;
	int
	 countTasksInLocalHost() const;
	vector<uint>
	 getThreadsOfTasks() const;
};

#endif /* COMMUNICATIONSMPI_H_ */
