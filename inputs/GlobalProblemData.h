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

class GlobalProblemData {
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
	double finalTime; // Final time.
	double samplingPeriod;
	double scalingFactor; //
	double timeStep;
	uint numberOfProcesses;
	string hostsFile;
	pair<boundType,boundType> boundTermination[3];
	pair<CVecD3,CVecD3> boundaryPadding, boundaryMeshSize;
	GlobalProblemData();
	GlobalProblemData&
	 operator=(const GlobalProblemData& rhs);
	void
	 applyGeometricScalingFactor(const double& factor);
	void
	 printInfo() const;
private:
	void
	 printBoundType(const boundType) const;
};

#endif /* GLOBALPROBLEMDATA_H_ */
