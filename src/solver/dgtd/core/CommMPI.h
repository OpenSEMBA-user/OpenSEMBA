// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
///*
// * CommunicationsMPI.h
// *
// *  Created on: Apr 17, 2013
// *      Author: luis
// */

#ifndef COMMUNICATIONSMPI_H_
#define COMMUNICATIONSMPI_H_

//#include <vector>
//#include <mpi.h>
//
//#include "Comm.h"
//
////#define USE_OLD_SYNC
//
//#ifndef MESH_ALLOW_PARTITIONING
//	#error "MPI communications need a mesh partitioning method."
//#endif
//
//struct Field_s {
//	Int rp;
//	Real Ex, Ey, Ez, Hx, Hy, Hz;
//};
//
//class CommMPI : public Comm {
//public:
//	CommMPI();
//	virtual ~CommMPI();
//	Int
//	 getNumberOfTasks() const;
//	Int
//	 getTask() const;
//	UInt
//	 getLocalOffset() const;
//	UInt
//	 getLocalSize() const;
//	Int
//	 getNumOfTasksOnThisHost() const;
//	void
//	 setPartitionSizes(
//	  const vector<vector<UInt> >& partId);
//	void
//	 gatherFieldsMaster(
// 	  Real* Ex, Real* Ey, Real* Ez,
//	  Real* Hx, Real* Hy, Real* Hz,
//	  const Real* lEx, const Real* lEy, const Real* lEz,
//	  const Real* lHx, const Real* lHy, const Real* lHz) const;
//	void
//	 gatherFieldsSlave(
// 	  const Real* Ex, const Real* Ey, const Real* Ez,
//	  const Real* Hx, const Real* Hy, const Real* Hz) const;
//	bool
//	 isMaster() const;
//	void
//	 syncNeighbourFields(
//	  Real* nEx, Real* nEy, Real* nEz,
//	  Real* nHx, Real* nHy, Real* nHz,
//	  const Real* Ex, const Real* Ey, const Real* Ez,
//	  const Real* Hx, const Real* Hy, const Real* Hz) const;
//	void
//	 initNeighbourFields(const vector<UInt>& nIds);
//	Real
//	 reduceToGlobalMinimum(Real val) const;
//	void
//	 printInfo() const;
//private:
//	static const Int master = 0;
//	static const UInt np = ((ORDER_N+1)*(ORDER_N+2)*(ORDER_N+3)/6);
//	static const bool weightPartitions = true;
//	Int nTasks;
//	Int nTasksInHost;
//	Int task;
//	Int len;
//	Int* pSize;
//	Int* pOffset;
//	Int* fSize;
//	Int* fOffset;
//	UInt* neighElemRP;
//	MPI_Comm world;
//	MPI_Datatype MPIField;
//	UInt nPartitions;
//	Int* nNeighId;
//	UInt** neighId;
//	Int **neighFSize;
//	Int **neighFOffset;
//	Int **nDofSize;
//	Int **nDofOffset;
//	Int*
//	 getDofSizes() const;
//	Int*
//	 getDofOffsets() const;
//	UInt
//	 getLocalDofSize() const;
//	UInt
//	 getGlobalDofSize() const;
//	void
//	 packFields(
//	  Field_s *field,
//	  const Real *Ex, const Real *Ey, const Real *Ez,
//	  const Real *Hx, const Real *Hy, const Real *Hz,
//	  const UInt fSize) const;
//	void
//	 unpackFields(
//	  Real *Ex, Real *Ey, Real *Ez,
//	  Real *Hx, Real *Hy, Real *Hz,
//	  const Field_s *field, const UInt fSize) const;
//	void
//	 commitFieldStruct();
//	Int*
//	 getFieldSizes() const;
//	Int*
//	 getFieldOffsets() const;
//	UInt
//	 getLocalFieldSize() const;
//	Int
//	 getTaskOfId(const UInt id) const;
//	bool
//	 checkNNeighCoherence(Int* nneigh) const;
//	bool
//	 checkVectorsAreEqual(
//	  const UInt vSize,
//	  const UInt* v,
//	  const vector<UInt>& nIds) const;
//	bool
//	 checkNeighFSizes() const;
//	Int
//	 countTasksInLocalHost() const;
//	vector<UInt>
//	 getThreadsOfTasks() const;
//};
//
//#endif /* COMMUNICATIONSMPI_H_ */
