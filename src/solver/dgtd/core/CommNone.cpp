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

Int
CommNone::getNumberOfTasks() const {
   return 1;
}

void CommNone::abort() const {

}

UInt CommNone::getLocalOffset() const {
   return 0;
}

UInt CommNone::getLocalSize() const {
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
      const vector<vector<ElementId>>& partId) {
   assert(partId.size() == 1);
   setGlobalSize(partId[0].size());
   setLocalSizeAndOffset(getGlobalSize(), 0);
}

void
CommNone::syncNeighbourFields(Real* nEx, Real* nEy, Real* nEz,
      Real* nHx, Real* nHy, Real* nHz, const Real* Ex, const Real* Ey,
      const Real* Ez, const Real* Hx, const Real* Hy,
      const Real* Hz) const {

}

void
CommNone::initNeighbourFields(const vector<UInt>& nIds) {

}

Int
CommNone::getTask() const {
   return 0;
}

Real
CommNone::reduceToGlobalMinimum(Real val) const {
   return val;
}

Int
CommNone::getNumOfTasksOnThisHost() const {
   return 1;
}

void
CommNone::printInfo() const {
   cout << "---- CommNone Info ----" <<endl;
   cout << "No multiprocessor comunications being used." << endl;
}
