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

typedef unsigned int uint;

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

    double getFinalTime() const;
    void setFinalTime(double finalTime);
    double getSamplingPeriod() const;
    void setSamplingPeriod(double samplingPeriod);
    Solver getSolver() const;
    void setSolver(Solver solver);
    double getTimeStep() const;
    void setTimeStep(double timeStep);

    void printInfo() const;
private:
    // Global
    Solver solver_;
    double finalTime_;
    uint numberOfTimeSteps_;
    double samplingPeriod_;
    double timeStep_;
    double cfl_;
    // ugrfdtd
    CompositeModel compositeModel_;

    //
    string toStr(const Solver solver) const;
};

#endif /* GLOBALPROBLEMDATA_H_ */
