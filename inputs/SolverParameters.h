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
	void applyGeometricScalingFactor(const Real& factor);
	void printInfo() const;
    const pair<CVecR3, CVecR3>& getBoundaryMeshSize() const;
    void setBoundaryMeshSize(const pair<CVecR3, CVecR3>& boundaryMeshSize);
    const pair<CVecR3, CVecR3>& getBoundaryPadding() const;
    void setBoundaryPadding(const pair<CVecR3, CVecR3>& boundaryPadding);
    Real getFinalTime() const;
    void setFinalTime(Real finalTime);
    Real getSamplingPeriod() const;
    void setSamplingPeriod(Real samplingPeriod);
    Real getScalingFactor() const;
    void setScalingFactor(Real scalingFactor);
    Solver getSolver() const;
    void setSolver(Solver solver);
    Real getTimeStep() const;
    void setTimeStep(Real timeStep);
    pair<boundType,boundType> getBoundTermination(const UInt i) const;
    void setBoundTermination(const UInt i, UInt j, boundType bound);
private:
	Solver solver_;
	Real finalTime_;
	Real samplingPeriod_;
	Real scalingFactor_;
	Real timeStep_;
	pair<boundType,boundType> boundTermination_[3];
	pair<CVecR3,CVecR3> boundaryPadding_, boundaryMeshSize_;
	string toStr(const boundType) const;
	string toStr(const Solver solver) const;
};

#endif /* GLOBALPROBLEMDATA_H_ */
