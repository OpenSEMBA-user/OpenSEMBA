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

class SolverParameters {
public:
	typedef enum {
		pec,
		pmc,
		pml,
		mur1,
		mur2,
		periodic,
		undefined
	} boundType;
	typedef enum {
	    ugrfdtd,
	    cudg3d,
	    none
	} Solver;
	SolverParameters();
	void applyGeometricScalingFactor(const double& factor);
	void printInfo() const;
    const pair<CVecD3, CVecD3>& getBoundaryMeshSize() const;
    void setBoundaryMeshSize(const pair<CVecD3, CVecD3>& boundaryMeshSize);
    const pair<CVecD3, CVecD3>& getBoundaryPadding() const;
    void setBoundaryPadding(const pair<CVecD3, CVecD3>& boundaryPadding);
    double getFinalTime() const;
    void setFinalTime(double finalTime);
    double getSamplingPeriod() const;
    void setSamplingPeriod(double samplingPeriod);
    double getScalingFactor() const;
    void setScalingFactor(double scalingFactor);
    Solver getSolver() const;
    void setSolver(Solver solver);
    double getTimeStep() const;
    void setTimeStep(double timeStep);
    pair<boundType,boundType> getBoundTermination(const uint i) const;
    void setBoundTermination(const uint i, uint j, boundType bound);
private:
	Solver solver_;
	double finalTime_;
	double samplingPeriod_;
	double scalingFactor_;
	double timeStep_;
	pair<boundType,boundType> boundTermination_[3];
	pair<CVecD3,CVecD3> boundaryPadding_, boundaryMeshSize_;
	string toStr(const boundType) const;
	string toStr(const Solver solver) const;
};

#endif /* GLOBALPROBLEMDATA_H_ */
