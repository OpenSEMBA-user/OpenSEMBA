/*
 * Communications.h
 *
 *  Created on: Apr 17, 2013
 *      Author: luis
 */

#ifndef COMMNONE_H_
#define COMMNONE_H_

#include "stdlib.h"
#include <iostream>
#include <vector>
#include <assert.h>
#include "Comm.h"

using namespace std;

#define COMMUNICATIONS_SUCCESS 0
#define COMMUNICATIONS_ERROR 10334

class CommNone : public Comm {
public:
	CommNone();
	virtual ~CommNone();
	int
	 getNumberOfTasks() const;
	void
	 abort() const;
	bool
	 isMaster() const;
	int
	 getTask() const;
	uint
	 getLocalOffset() const;
	int
	 getNumOfTasksOnThisHost() const;
	uint
	 getLocalSize() const;
	void
	 gatherFieldsMaster(
 	  FieldD3& electric,
	  FieldD3& magnetic,
	  const FieldD3& localElectric,
	  const FieldD3& localMagnetic) const;
	void
	 gatherFieldsSlave(
 	  const FieldD3& electric,
	  const FieldD3& magnetic) const;
	void
	 setPartitionSizes(
	  const vector<vector<uint> >& partId);
	void
	 syncNeighbourFields(
	  double* nEx, double* nEy, double* nEz,
	  double* nHx, double* nHy, double* nHz,
	  const double* Ex, const double* Ey, const double* Ez,
	  const double* Hx, const double* Hy, const double* Hz) const;
	double
	 reduceToGlobalMinimum(double val) const;
	void
	 initNeighbourFields(const vector<uint>& nIds);
	void
	 printInfo() const;
};

#endif
