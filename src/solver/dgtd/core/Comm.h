/*
 * Communications.h
 *
 *  Created on: Apr 17, 2013
 *      Author: luis
 */

#ifndef COMMUNICATIONS_H_
#define COMMUNICATIONS_H_

#include "stdlib.h"
#include "MeshVolume.h"
#include "Ordering.h"
#include "Field.h"
#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

class Comm : public Ordering {
public:
	virtual ~Comm();
	virtual Int
	 getNumberOfTasks() const = 0;
	virtual Int
	 getTask() const = 0;
	virtual bool
	 isMaster() const = 0;
	virtual Int
	 getNumOfTasksOnThisHost() const = 0;
	virtual UInt
	 getLocalOffset() const = 0;
	virtual UInt
	 getLocalSize() const = 0;
	virtual void
	 setPartitionSizes(
      const vector<vector<UInt> >& partId) = 0;
	virtual void
	 gatherFieldsMaster(
 	  FieldR3& elec, FieldR3& magn,
 	  const FieldR3& localElec, const FieldR3& localMagn) const = 0;
	virtual void
	 gatherFieldsSlave(
 	  const FieldR3& electric, const FieldR3& magnetic) const = 0;
	virtual void
	 syncNeighbourFields(
	  Real* nEx, Real* nEy, Real* nEz,
	  Real* nHx, Real* nHy, Real* nHz,
	  const Real* Ex, const Real* Ey, const Real* Ez,
	  const Real* Hx, const Real* Hy, const Real* Hz) const = 0;
	virtual void
	 initNeighbourFields(const vector<UInt>& nIds) = 0;
	virtual Real
	 reduceToGlobalMinimum(Real val) const = 0;
	virtual void
	 printInfo() const = 0;
};

#endif /* COMMUNICATIONS_H_ */
