/*
 * Solver.h
 *
 *  Created on: Jan 19, 2015
 *      Author: luis
 */

#ifndef SRC_SOLVER_H_
#define SRC_SOLVER_H_

#include <iostream>
#include <time.h>
#include <sys/time.h>

using namespace std;

class Solver {
public:
   Solver();
   virtual ~Solver();
   virtual void run() = 0;
protected:
   static const uint printStep = 100;
   void
    printTime(const double originalSeconds) const;
   double
    storeCPUTime() const;
   void
    printTimeProfilingInfo(
     double tSum,
     double tRunning,
     const uint tStep,
     const uint Ntsteps) const;
};

#endif /* SRC_SOLVER_H_ */
