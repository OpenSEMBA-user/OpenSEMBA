
/*
 * Arguments.cpp
 *
 *  Created on: Aug 24, 2012
 *      Author: luis
 */
#include "OptionsSolverDGTD.h"

OptionsSolverDGTD::OptionsSolverDGTD() {
	initDefaultValues();
}

OptionsSolverDGTD::~OptionsSolverDGTD() {

}

OptionsSolverDGTD::OptionsSolverDGTD(int argc_, char* argv[]) {
	initDefaultValues();
	path_ = *argv++;
	if (argc == 1) {
		cout<< " >>>> No arguments where given <<<< " << endl;
		printHelp();
		exit(EXIT_FAILURE);
	}
	for (int i = 1; i < argc; i++) {
		string str = *argv++;
		if (!str.compare("-n")) { // "-n" file name
			fileName_ = getArgvpp(++i, *argv++);
		} else if (!str.compare("-t")) { // "-t" problem type Path.
			problemTypePath = getArgvpp(++i, *argv++);
			problemTypePath += "/";
		} else if (!str.compare("-o")) {
			outputFolderName = getArgvpp(++i, *argv++);
		} else if (!str.compare("--resume")) {
			resume = true;
		} else if (!str.compare("--dontRun")) {
			dontRun = true;
		} else if (!str.compare("--timeIntegrator")) {
			str = getArgvpp(++i, *argv++);
			timeIntegrator = getTimeIntegratorFromStr(str);
		} else if (!str.compare("--noLTS")) {
			noLTS = true;
		} else if (!str.compare("--useMaxStageSizeForLTS")) {
			useMaxStageSizeForLTS = true;
		} else if (!str.compare("--timeStepSize")
		 || !str.compare("--cfl")) {
			str = getArgvpp(++i, *argv++);
			timeStepSize = atof(str.c_str());
		} else if (!str.compare("--growSmallerTiers")) {
			str = getArgvpp(++i, *argv++);
			growSmallerTiers = atoi(str.c_str());
		} else if (!str.compare("--maxNumberOfTiers")
		 || !str.compare("--maxNumOfTiers")) {
			str = getArgvpp(++i, *argv++);
			maxNumberOfTiers = atoi(str.c_str());
		} else if (!str.compare("--upwinding")) {
			str = getArgvpp(++i, *argv++);
			upwinding = atof(str.c_str());
		} else if (!str.compare("--linearizeMesh")) {
			linearizeMesh = true;
		} else if (!str.compare("--PMLIgnore")) {
			PMLIgnore = true;
		} else if (!str.compare("--PMLUseConstantConductivity")
		 || !str.compare("--PMLConstant")) {
			PMLUseConstantConductivity = true;
		} else if (!str.compare("--PMLConductivity")) {
			str = getArgvpp(++i, *argv++);
			PMLConductivity = atof(str.c_str());
		} else if (!str.compare("-h") || !str.compare("--help")) {
			printHelp();
			abort(EXIT_SUCCESS);
		} else {
			cout<< "Input argument not reckognized: " << str << endl;
			printHelp();
			abort(ARGUMENTS_ERROR);
		}
	}
}

void
OptionsSolverDGTD::printInfo() const {
	cout<< " -- Arguments info -- " << endl;
	cout<< "Solver path: " << path_ << endl;
	cout<< "Project name: " << getProjectName() << endl;
	cout<< "Project path: " << getProjectFolder() << endl;
	cout<< "Problem type path: " << problemTypePath << endl;
	cout<< "Input file name: " << fileName_ << endl;
	cout<< "Output folder: " << getOutputFolder() << endl;
	cout<< " -- Options -- " << endl;
	cout<< "Resume: " << resume << endl;
	cout<< "Dont Run: " << dontRun << endl;
	cout<< " -- Spatial discretization --" << endl;
	cout<< "Upwinding: " << upwinding << endl;
	cout<< "Linearizing mesh: " << linearizeMesh << endl;
	cout<< " -- PML options -- " << endl;
	cout<< "Ignore PMLs: " << PMLIgnore << endl;
	cout<< "Use Constant conductivity: "
	 << PMLUseConstantConductivity << endl;
	cout<< "Conductivity set to: " << PMLConductivity << endl;
 	cout<< " -- Time integration --" << endl;
	cout<< "Integrator: ";
	switch (timeIntegrator) {
	case lserk4:
		cout<< "4th Order Low-Storage Explicit Runge-Kutta" << endl;
		break;
	case verlet:
		cout<< "Verlet scheme" << endl;
		break;
	case lf2:
		cout<< "2nd Order Leapfrog (semi-defined)" << endl;
		break;
	case lf2full:
		cout<< "2nd Order Leapfrog (fully defined)" << endl;
		break;
	default:
		cout<< "Unknown" << endl;
		break;
	}
	cout<< "No LTS: " << noLTS << endl;
	cout<< "Time step size: " << timeStepSize << endl;
	cout<< "Use max. stage size: " << useMaxStageSizeForLTS << endl;
	cout<< "Grow smaller tiers: " << growSmallerTiers << endl;
	cout<< "Max. number of tiers: ";
	if (maxNumberOfTiers == 0) {
		cout<< "Unlimited" << endl;
	} else {
		cout<< maxNumberOfTiers << endl;
	}

}

bool OptionsSolverDGTD::isDontRun() const {
	return dontRun;
}

uint OptionsSolverDGTD::getGrowSmallerTiers() const {
	return growSmallerTiers;
}

bool OptionsSolverDGTD::isLinearizeMesh() const {
	return linearizeMesh;
}

uint OptionsSolverDGTD::getMaxNumberOfTiers() const {
	return maxNumberOfTiers;
}

bool OptionsSolverDGTD::isNoLTS() const {
	return noLTS;
}

string OptionsSolverDGTD::getOutputFolder() const {
	return getProjectFolder() + outputFolderName + "/";
}

double OptionsSolverDGTD::getPmlConductivity() const {
	return PMLConductivity;
}

bool OptionsSolverDGTD::isPmlIgnore() const {
	return PMLIgnore;
}

bool OptionsSolverDGTD::isPmlUseConstantConductivity() const {
	return PMLUseConstantConductivity;
}

string OptionsSolverDGTD::getProblemTypePath() const {
	return problemTypePath;
}

bool OptionsSolverDGTD::isResume() const {
	return resume;
}

OptionsSolverDGTD::TimeIntegrator
OptionsSolverDGTD::getTimeIntegrator() const {
	return timeIntegrator;
}

double OptionsSolverDGTD::getTimeStepSize() const {
	return timeStepSize;
}

double OptionsSolverDGTD::getUpwinding() const {
	return upwinding;
}

bool OptionsSolverDGTD::isUseMaxStageSizeForLts() const {
	return useMaxStageSizeForLTS;
}

void
OptionsSolverDGTD::initDefaultValues() {
	resume = false;
	dontRun = false;
	timeIntegrator = lserk4;
	useMaxStageSizeForLTS = false;
	noLTS = false;
	growSmallerTiers = 0;
	maxNumberOfTiers = 0;
	timeStepSize = 1.0;
	upwinding = 1.0;
	linearizeMesh = false;
	PMLIgnore = false;
	PMLUseConstantConductivity = false;
	PMLConductivity = 0.0;
}

void
OptionsSolverDGTD::printHelp() const {
	cout<< ">>>>>> cudg3d usage <<<<<<" << endl;
	cout<< " === Synopsis === " << endl;
	cout<< "cudg3d [OPTIONS] -n projectFolder -t problemTypeFolder " << endl;
	cout<< " -n <name>" << endl;
	cout<< "     Specifies an input file."<< endl;
	cout<< " -t <folder> " << endl;
	cout<< "     Specifes a path to problem type folder." << endl;
	cout<< " === General options ===" << endl;
	cout<< " -h or --help" << endl;
	cout<< "     Prints this help and exits." << endl;
	cout<< " -o <name>" << endl;
	cout<< "     Specifies a name for output file." << endl;
	cout<< " --resume " << endl;
	cout<< "     Looks for a .resume file to resume the simulation." << endl;
	cout<< " --dontRun " << endl;
	cout<< "     Intializes from file, writes .post.msh and exits." << endl;
	cout<< " === Time integrator options === " << endl;
	cout<< " --timeIntegrator <type> (defaults to lserk4)" << endl;
	cout<< "     lserk4  4th Order Low-Storage Explicit Runge-Kutta." << endl;
	cout<< "     verlet  2nd Order Verlet scheme." << endl;
	cout<< "     lf2     2nd Order Leapfrog scheme." << endl;
	cout<< "     lf2full 2nd Order LF. Completely defined states" << endl;
	cout<< " --noLTS " << endl;
	cout<< "     Deactivates Local Time Stepping." << endl;
	cout<< " --useMaxStageSizeForLTS" << endl;
	cout<< "     LTS will be done using max stage size of lserk4 as "
     << "ratio between time tiers. Default is (1 / Nstages). " << endl;
	cout<< " --timeStepSize <value from 0.0 to 1.0> (def. to 1.0)" << endl;
	cout<< "     1.0     Max. warranted stability." << endl;
	cout<< "    >1.0     Unsafe speed-up." << endl;
	cout<< " --maxNumberOfTiers <num>, --maxNumOfTiers <num>" << endl;
	cout<< "     Sets a maximum number of tiers for LTS." << endl;
	cout<< " --growSmallerTiers <num>" << endl;
	cout<< "     Includes <num> neighbours in smaller tiers." << endl;
	cout<< " === Spatial discretization options ===" << endl;
	cout<< " --upwinding <value from 0.0 to 1.0> (defaults to 1.0)" << endl;
	cout<< "     1.0     Full flux upwinding (Riemannian flux)." << endl;
	cout<< "     0.0     Centred flux." << endl;
	cout<< "  (0.0,1.0)  Partially penalized flux" << endl;
	cout<< " --linearizeMesh" << endl;
	cout<< "     Treats all curvilinear elements as linear." << endl;
	cout<< " >>>> AND MORE <<<< " << endl;
}

OptionsSolverDGTD::TimeIntegrator
OptionsSolverDGTD::getTimeIntegratorFromStr(
 const string& str) const {
	if (!str.compare("lserk4")) {
		return lserk4;
	} else if (!str.compare("verlet")) {
		return verlet;
	} else if (!str.compare("lf2")) {
		return lf2;
	} else if (!str.compare("lf2full")) {
		return lf2full;
	} else {
		printHelp();
		cerr << endl << "ERROR @ Arguments: ";
		cerr << endl << "Unreckognized time integrator: " << str << endl;
		exit(ARGUMENTS_ERROR);
	}
}

string
OptionsSolverDGTD::getOutputFolderName() const {
   if (outputFolderName.empty()) {
      return getProjectName();
   } else {
      return outputFolderName;
   }
}
