/*
 * GlobalProblemData.h
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#ifndef GLOBALPROBLEMDATA_H_
#define GLOBALPROBLEMDATA_H_

#include <iostream>
#include <utility>
#include "../math/CartesianVector.h"
#include "Options.h"

using namespace std;

class SolverOptions : public Options {
public:
    enum class Solver {
        ugrfdtd, cudg3d, none
    };
    enum class CompositeModel {
        Default, digFilt, MIBC, ADEMIBC, URMMMT
    };
    enum class MetalModel {
        Default, maloney, maloneySkinDepth, conformalSkinDepth
    };
    enum class WireModel {
        Default, transition, New
    };
    enum class SelfInductanceModel {
        boutayeb, ledfelt, berenger
    };
    SolverOptions();

    Real getFinalTime() const;
    void setFinalTime(Real finalTime);
    Real getSamplingPeriod() const;
    void setSamplingPeriod(Real samplingPeriod);
    Solver getSolver() const;
    void setSolver(Solver solver);
    Real getTimeStep() const;
    void setTimeStep(Real timeStep);

    void printInfo() const;
private:
    // Global
    Solver solver_;
    Real finalTime_;
    UInt numberOfTimeSteps_;
    Real samplingPeriod_;
    Real timeStep_;
    Real cfl_;
    // ugrfdtd
    CompositeModel compositeModel_;

    //
    string toStr(const Solver solver) const;
};

#endif /* GLOBALPROBLEMDATA_H_ */
