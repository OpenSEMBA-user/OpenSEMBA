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

//void Integrator::processStopRequest() {
//  // Terminates the program storing the fields if key is pressed.
//  nonblock(NB_ENABLE);
//  Int endingRequest = 0;
//  endingRequest = kbhit();
//  if (endingRequest != 0 && fgetc(stdin)=='q') {
//      cout << "Key 'q' was pressed, terminating." << endl;
//      // Writes fields for resume.
//      cout << " - Writing resume files." << endl;
//      const Real *fPtrs[6];
//      getFieldPtrs(fPtrs);
//      out->writeResumeFile(fPtrs, tStep, nK);
//      cout << " - Program was succesfully terminated." << endl;
//      exit(EXIT_SUCCESS);
//  }
//  nonblock(NB_DISABLE);
//}
//
//Int Integrator::kbhit()   {
//  struct timeval tv;
//  fd_set fds;
//  tv.tv_sec = 0;
//  tv.tv_usec = 0;
//  FD_ZERO(&fds);
//  FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
//  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
//  return FD_ISSET(STDIN_FILENO, &fds);
//}
//
//void Integrator::nonblock(Int state) {
//  struct termios ttystate;
//  //get the terminal state
//  tcgetattr(STDIN_FILENO, &ttystate);
//  if (state == NB_ENABLE) {
//      //turn off canonical mode
//      ttystate.c_lflag &= ~ICANON;
//      //minimum of number input read.
//      ttystate.c_cc[VMIN] = 1;
//  }
//  else if (state == NB_DISABLE) {
//      //turn on canonical mode
//      ttystate.c_lflag |= ICANON;
//  }
//  //set the terminal attributes.
//  tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
//}
