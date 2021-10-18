#pragma once

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

    Math::Real getFinalTime() const;
    Math::Real getSamplingPeriod() const;
    Math::Real getCFL() const;
    Math::Real getTimeStep() const;
    Math::Real getFlush() const;
    bool isForceRestarting() const;
    std::size_t getNumberOfTimeSteps() const;
    const std::string& getAdditionalArguments() const;
    bool isResumeSimulation() const;

    std::string toStr() const;

    virtual std::string toArgsStr() const;
    bool isRunSimulation() const;
    void setRunSimulation(bool runSimulation);

private:

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

