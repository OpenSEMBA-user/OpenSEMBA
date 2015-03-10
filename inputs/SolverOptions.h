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

using namespace std;

typedef unsigned int uint;

class SolverOptions : public Options {
public:
	typedef enum {
	    ugrfdtd,
	    cudg3d,
	    none
	} Solver;
	typedef enum {

	} CompositesModel;
    typedef enum {

    } MetalsModel;
    typedef enum {

    } WiresModel;
    typedef enum {

    } WiresSelfinductance;
    SolverOptions();
	void printInfo() const;
    double getFinalTime() const;
    void setFinalTime(double finalTime);
    double getSamplingPeriod() const;
    void setSamplingPeriod(double samplingPeriod);
    Solver getSolver() const;
    void setSolver(Solver solver);
    double getTimeStep() const;
    void setTimeStep(double timeStep);
private:
    // Global
	Solver solver_;
	double finalTime_;
	uint numberOfTimeSteps_;
	double samplingPeriod_;
	double timeStep_;
	double cfl_;
	string toStr(const Solver solver) const;
};

#endif /* GLOBALPROBLEMDATA_H_ */
