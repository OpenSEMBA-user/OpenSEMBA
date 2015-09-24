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
 * Solver.h
 *
 *  Created on: Jan 19, 2015
 *      Author: luis
 */

#ifndef SRC_SOLVER_H_
#define SRC_SOLVER_H_

#include <ctime>
#include <iostream>
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
    void printTime(const double originalSeconds) const;
    double storeCPUTime() const;
    void printTimeProfilingInfo(
            double tSum,
            double tRunning,
            const UInt tStep,
            const UInt Ntsteps) const;
    //   void
    //   processStopRequest();
    //  Int
    //   kbhit();
    //  void
    //   nonblock(Int state);
};

#endif /* SRC_SOLVER_H_ */
