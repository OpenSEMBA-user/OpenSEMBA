/*
 * Arguments.h
 *
 *  Created on: Aug 24, 2012
 *      Author: luis
 */

#ifndef ARGUMENTSDGTD_H_
#define ARGUMENTSDGTD_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <omp.h>
#include "Arguments.h"

using namespace std;
typedef unsigned int uint;

class ArgumentsDGTD : public Arguments {
public:
	typedef enum  {
		lserk4, verlet, lf2, lf2full
	} TimeIntegrator;
	ArgumentsDGTD();
	ArgumentsDGTD(int argc, char* argv[]);
	virtual ~ArgumentsDGTD();
	void printInfo() const;
	string getOutputFolderName() const;
	bool isDontRun() const;
	uint getGrowSmallerTiers() const;
	bool isLinearizeMesh() const;
	uint getMaxNumberOfTiers() const;
	bool isNoLTS() const;
	string getOutputFolder() const;
	double getPmlConductivity() const;
	bool isPmlIgnore() const;
	bool isPmlUseConstantConductivity() const;
	string getProblemTypePath() const;
	bool isResume() const;
	TimeIntegrator getTimeIntegrator() const;
	double getTimeStepSize() const;
	double getUpwinding() const;
	bool isUseMaxStageSizeForLts() const;
private:
	string outputFolderName;
	string problemTypePath;
	TimeIntegrator timeIntegrator;
	bool resume;
	bool dontRun;
	bool noLTS;
	double timeStepSize;
	double upwinding;
	bool linearizeMesh;
	bool PMLIgnore;
	bool PMLUseConstantConductivity;
	double PMLConductivity;
	uint growSmallerTiers;
	uint maxNumberOfTiers;
	bool useMaxStageSizeForLTS;
	TimeIntegrator
	 getTimeIntegratorFromStr(
	  const string& TimeIntegratorStr) const;
	void
	 printHelp() const;
	void
	 initDefaultValues();
};
#endif /* ARGUMENTS_H_ */
