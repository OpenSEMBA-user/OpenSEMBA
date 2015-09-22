// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * Arguments.h
 *
 *  Created on: Aug 24, 2012
 *      Author: luis
 */

#ifndef ARGUMENTSDGTD_H_
#define ARGUMENTSDGTD_H_

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include <omp.h>

#include "Options.h"

using namespace std;
typedef unsigned int uint;

class OptionsSolverDGTD : public Options {
public:
	typedef enum  {
		lserk4, verlet, lf2, lf2full
	} TimeIntegrator;
	OptionsSolverDGTD();
	OptionsSolverDGTD(int argc, char* argv[]);
	virtual ~OptionsSolverDGTD();
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
