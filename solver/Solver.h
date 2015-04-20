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

#include "Types.h"

class Solver {
public:
   Solver();
   virtual ~Solver();
   virtual bool run() = 0;
   virtual bool canRun() const = 0;
protected:
   static const UInt printStep = 100;
   void
    printTime(const double originalSeconds) const;
   double
    storeCPUTime() const;
   void
    printTimeProfilingInfo(
     double tSum,
     double tRunning,
     const UInt tStep,
     const UInt Ntsteps) const;
};

#endif /* SRC_SOLVER_H_ */
