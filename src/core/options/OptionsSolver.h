/*
 * GlobalProblemData.h
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#ifndef GLOBALPROBLEMDATA_H_
#define GLOBALPROBLEMDATA_H_

#include <cmath>
#include <iostream>
#include <utility>

using namespace std;

#include "Options.h"
#include "math/CartesianVector.h"

class OptionsSolver : public Options {
public:
    enum class EndingCondition {
        finalTime, numberOfTimeSteps
    };
    enum class Solver {
        ugrfdtd, cudg3d, none
    };
    OptionsSolver();
    virtual ~OptionsSolver();

    DEFINE_CLONE(OptionsSolver);

    void set(const Arguments& args);

    void setFinalTime(Real finalTime);
    void setSamplingPeriod(Real samplingPeriod);
    void setCFL(double cfl);
    void setNumberOfTimeSteps(UInt numberOfTimeSteps);
    void setFlush(Real flush);
    void setForceRestarting(bool forceRestarting);
    void setTimeStep(Real timeStep);
    void setResumeSimulation(bool resumeSimulation);
    void setAdditionalArguments(const string& additionalArguments);

    Real getFinalTime() const;
    Real getSamplingPeriod() const;
    Real getCFL() const;
    Real getTimeStep() const;
    Real getFlush() const;
    bool isForceRestarting() const;
    UInt getNumberOfTimeSteps() const;
    const string& getAdditionalArguments() const;
    bool isResumeSimulation() const;

    void printInfo() const;
    void printHelp() const;

    static string toStr(const OptionsSolver::Solver& solver);
    virtual string toArgsStr() const;
private:
    // Global
    Solver solver_;

    EndingCondition endingCondition_;
    Real ending_;
    Real timeStep_;
    Real cfl_;

    Real samplingPeriod_;
    bool forceRestarting_;
    bool resumeSimulation_;
    bool dontRun_;
    Real flush_;
    string additionalArguments_;
};

#endif /* GLOBALPROBLEMDATA_H_ */
