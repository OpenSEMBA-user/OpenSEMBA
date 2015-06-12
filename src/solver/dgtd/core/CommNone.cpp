/*
 * Communications.cpp
 *
 *  Created on: Apr 17, 2013
 *      Author: luis
 */

#include "CommNone.h"

CommNone::CommNone() {
}

CommNone::~CommNone() {
}

int
CommNone::getNumberOfTasks() const {
   return 1;
}

void
CommNone::abort() const {

}

uint
CommNone::getLocalOffset() const {
   return 0;
}

uint
CommNone::getLocalSize() const {
   return getGlobalSize();
}

bool
CommNone::isMaster() const {
   return true;
}

void
CommNone::gatherFieldsMaster(
      FieldR3& electric,
      FieldR3& magnetic,
      const FieldR3& localElectric,
      const FieldR3& localMagnetic) const {
}

void
CommNone::gatherFieldsSlave(
      const FieldR3& electric,
      const FieldR3& magnetic) const {
   //
}

void
CommNone::setPartitionSizes(
      const vector<vector<uint> >& partId) {
   assert(partId.size() == 1);
   setGlobalSize(partId[0].size());
   setLocalSizeAndOffset(getGlobalSize(), 0);
}

void
CommNone::syncNeighbourFields(double* nEx, double* nEy, double* nEz,
      double* nHx, double* nHy, double* nHz, const double* Ex, const double* Ey,
      const double* Ez, const double* Hx, const double* Hy,
      const double* Hz) const {

}

void
CommNone::initNeighbourFields(const vector<uint>& nIds) {

}

int
CommNone::getTask() const {
   return 0;
}

double
CommNone::reduceToGlobalMinimum(double val) const {
   return val;
}

int
CommNone::getNumOfTasksOnThisHost() const {
   return 1;
}

void
CommNone::printInfo() const {
   cout << "---- CommNone Info ----" <<endl;
   cout << "No multiprocessor comunications being used." << endl;
}
