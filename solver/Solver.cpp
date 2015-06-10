/*
 * Solver.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: luis
 */

#include "Solver.h"

Solver::Solver() {
}

Solver::~Solver() {
}

double
Solver::storeCPUTime() const {
   timeval tim;
   gettimeofday(&tim, NULL);
   return tim.tv_sec + (tim.tv_usec/1000000.0);
}

void
Solver::printTimeProfilingInfo(
 double tSum,
 double tRunning,
 const UInt tStep,
 const UInt NtSteps) const {
   if (tStep % printStep == 0) {
      // Prints information on how much took to complete the current
      // set of time steps.
      cout<< "Step " << tStep << "/" << NtSteps << " || ";
      // Makes prediction on how much will take to complete.
      double tCPUFinal = 0.0;
      tRunning += tSum;
      tCPUFinal = tRunning * NtSteps / tStep;
      printTime(tCPUFinal - tRunning);
      cout<< " remaning." << endl;
   }
}

void
Solver::printTime(const double originalSeconds) const {
   int hours = 0;
   int minutes = 0;
   int secondsLeft = 0;
   hours = int(originalSeconds) / 3600;
   secondsLeft = originalSeconds - hours * 3600;
   minutes = int (secondsLeft) / 60;
   secondsLeft = secondsLeft - minutes * 60;
   if (hours > 0) {
      cout<< hours << "h ";
   } if (minutes > 0) {
      cout<< minutes << "m ";
   }
   cout<< secondsLeft << "s";
}
