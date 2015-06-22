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
	Int
	 getNumberOfTasks() const;
	void
	 abort() const;
	bool
	 isMaster() const;
	Int
	 getTask() const;
	UInt
	 getLocalOffset() const;
	Int
	 getNumOfTasksOnThisHost() const;
	UInt
	 getLocalSize() const;
	void
	 gatherFieldsMaster(
 	  FieldR3& electric,
	  FieldR3& magnetic,
	  const FieldR3& localElectric,
	  const FieldR3& localMagnetic) const;
	void
	 gatherFieldsSlave(
 	  const FieldR3& electric,
	  const FieldR3& magnetic) const;
	void
	 setPartitionSizes(
	  const vector<vector<UInt> >& partId);
	void
	 syncNeighbourFields(
	  Real* nEx, Real* nEy, Real* nEz,
	  Real* nHx, Real* nHy, Real* nHz,
	  const Real* Ex, const Real* Ey, const Real* Ez,
	  const Real* Hx, const Real* Hy, const Real* Hz) const;
	Real
	 reduceToGlobalMinimum(Real val) const;
	void
	 initNeighbourFields(const vector<UInt>& nIds);
	void
	 printInfo() const;
};

#endif
