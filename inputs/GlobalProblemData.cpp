/*
 * GlobalProblemData.cpp
 *
 *  Created on: Aug 27, 2012
 *      Author: luis
 */

#include "GlobalProblemData.h"

GlobalProblemData::GlobalProblemData () {
	finalTime = 0;
	samplingPeriod = 0.0;
	scalingFactor = 1.0;
	timeStep = 0.0;
	for (unsigned int i = 0; i < 3; i++) {
		boundTermination[i].first = pml;
		boundTermination[i].second = pml;
	}
	boundaryPadding = 0.0;
	boundaryMeshSize = 0.0;
	// Parallel run configuration.
	numberOfProcesses = 1;
}

GlobalProblemData&
GlobalProblemData::operator=(const GlobalProblemData& rhs) {
	if (&rhs == this) {
		return *this;
	}
	finalTime = rhs.finalTime;
	scalingFactor = rhs.scalingFactor;
	samplingPeriod = rhs.samplingPeriod;
	timeStep = rhs.timeStep;
	for (unsigned int i = 0; i < 3; i++) {
		boundTermination[i] = rhs.boundTermination[i];
	}
	boundaryPadding = rhs.boundaryPadding;
	boundaryMeshSize = rhs.boundaryMeshSize;
	numberOfProcesses = rhs.numberOfProcesses;
	hostsFile = rhs.hostsFile;
	return *this;
}

void
GlobalProblemData::printInfo() const {
	cout<< " --- Global data --- " << endl;
	cout<< "Final time: " << finalTime << endl;
	cout<< "Geom. scaling factor: " << scalingFactor << endl;
	cout<< "Default sampling period: " << samplingPeriod << endl;
	cout<< "Time step: " << timeStep << endl;
	cout<< "Lower X Bound ";
	printBoundType(boundTermination[0].first);
	cout<< endl;
	cout<< "Upper X Bound ";
	printBoundType(boundTermination[0].second);
	cout<< endl;
	cout<< "Lower Y Bound ";
	printBoundType(boundTermination[1].first);
	cout<< endl;
	cout<< "Upper Y Bound ";
	printBoundType(boundTermination[1].second);
	cout<< endl;
	cout<< "Lower Z Bound ";
	printBoundType(boundTermination[2].first);
	cout<< endl;
	cout<< "Upper Z Bound ";
	printBoundType(boundTermination[2].second);
	cout<< endl;
	cout<< "Boundary padding: " << boundaryPadding << endl;
	cout<< "Boundary Mesh size: " << boundaryMeshSize << endl;
	cout<< "- Parallel run parameters - " << endl;
	cout<< "Number of processes: " << numberOfProcesses << endl;
	cout<< "Hosts file: " << hostsFile << endl;
}

void
GlobalProblemData::printBoundType(
 const boundType val) const {
	switch (val) {
	case pec:
		cout<< "PEC";
		break;
	case pmc:
		cout<< "PMC";
		break;
	case pml:
		cout<< "PML";
		break;
	case mur1:
		cout<< "MUR1";
		break;
	case mur2:
		cout<< "MUR2";
		break;
	case periodic:
		cout<< "Periodic";
		break;
	case undefined:
		cout<< "Undefined";
		break;
	}
}
