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
	virtual int
	 getNumberOfTasks() const = 0;
	virtual int
	 getTask() const = 0;
	virtual bool
	 isMaster() const = 0;
	virtual int
	 getNumOfTasksOnThisHost() const = 0;
	virtual uint
	 getLocalOffset() const = 0;
	virtual uint
	 getLocalSize() const = 0;
	virtual void
	 setPartitionSizes(
      const vector<vector<uint> >& partId) = 0;
	virtual void
	 gatherFieldsMaster(
 	  FieldR3& elec, FieldR3& magn,
 	  const FieldR3& localElec, const FieldR3& localMagn) const = 0;
	virtual void
	 gatherFieldsSlave(
 	  const FieldR3& electric, const FieldR3& magnetic) const = 0;
	virtual void
	 syncNeighbourFields(
	  double* nEx, double* nEy, double* nEz,
	  double* nHx, double* nHy, double* nHz,
	  const double* Ex, const double* Ey, const double* Ez,
	  const double* Hx, const double* Hy, const double* Hz) const = 0;
	virtual void
	 initNeighbourFields(const vector<uint>& nIds) = 0;
	virtual double
	 reduceToGlobalMinimum(double val) const = 0;
	virtual void
	 printInfo() const = 0;
};

#endif /* COMMUNICATIONS_H_ */
