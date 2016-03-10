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

#ifndef SEMBA_SOLVER_FDTD_OPTIONS_H_
#define SEMBA_SOLVER_FDTD_OPTIONS_H_

#include <cmath>
#include <iostream>
#include <utility>

#include "math/vector/Cartesian.h"
#include "argument/Group.h"
#include "solver/Info.h"

namespace SEMBA {
namespace Solver {

class Options {
public:
    enum class EndingCondition {
        finalTime, numberOfTimeSteps
    };
    enum class Solver {
        ugrfdtd, cudg3d, none
    };
    Options();
    virtual ~Options();

    virtual void addArguments(Argument::Group& args) const;
    virtual void set(const Settings& args);

    void setFinalTime(Math::Real finalTime);
    void setSamplingPeriod(Math::Real samplingPeriod);
    void setCFL(double cfl);
    void setNumberOfTimeSteps(std::size_t numberOfTimeSteps);
    void setFlush(Math::Real flush);
    void setForceRestarting(bool forceRestarting);
    void setTimeStep(Math::Real timeStep);
    void setResumeSimulation(bool resumeSimulation);
    void setAdditionalArguments(const std::string& additionalArguments);
//    void setSolver(Solver solver);

    Math::Real getFinalTime() const;
    Math::Real getSamplingPeriod() const;
    Math::Real getCFL() const;
    Math::Real getTimeStep() const;
    Math::Real getFlush() const;
    bool isForceRestarting() const;
    std::size_t getNumberOfTimeSteps() const;
//    Solver getSolver() const;
    const std::string& getAdditionalArguments() const;
    bool isResumeSimulation() const;

    std::string toStr() const;
    void printInfo() const;
    void printHelp() const;

//    static std::string toStr(const Options::Solver& solver);
//    virtual std::string toArgsStr() const; // DEPRECATED
    bool isRunSimulation() const;
    void setRunSimulation(bool runSimulation);

private:
    // Global
//    Solver solver_;

    bool runSimulation_;
    EndingCondition endingCondition_;
    Math::Real finalTime_;
    Math::Real timeStep_;
    Math::Real cfl_;

    Math::Real samplingPeriod_;
    bool forceRestarting_;
    bool resumeSimulation_;
    Math::Real flush_;
    std::string additionalArguments_;
};

}
}

#endif /* SEMBA_SOLVER_FDTD_OPTIONS_H_ */
