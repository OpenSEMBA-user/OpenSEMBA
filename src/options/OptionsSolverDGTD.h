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
#include <omp.h>

using namespace std;

#include "OptionsSolver.h"

class OptionsSolverDGTD : public OptionsSolver {
public:
	typedef enum {
		lserk4, verlet, lf2, lf2full
	} TimeIntegrator;

	OptionsSolverDGTD();
	OptionsSolverDGTD(const OptionsSolver& base);
	virtual ~OptionsSolverDGTD();

    DEFINE_CLONE(OptionsSolverDGTD);

    void set(const Arguments& args);

    void setGrowSmallerTiers(UInt growSmallerTiers);
    void setMaxNumberOfTiers(UInt maxNumberOfTiers);
    void setPMLConductivity(Real pmlConductivity);
    void setPMLConstantConductivityProfile(bool);
    void setTimeIntegrator(TimeIntegrator timeIntegrator);
    void setUpwinding(Real upwinding);
    void setUseLTS(bool useLts);
    void setUseMaxStageSizeForLTS(bool useMaxStageSizeForLts);

	TimeIntegrator getTimeIntegrator() const;
	Real getUpwinding() const;
    UInt getGrowSmallerTiers() const;
    UInt getMaxNumberOfTiers() const;
    Real getPMLConductivity() const;
    bool isPMLConstantConductivityProfile() const;
    bool isUseLTS() const;
    bool isUseMaxStageSizeForLTS() const;

	static TimeIntegrator strToTimeIntegrator(const string& str);
	void printInfo() const;
private:
	Real upwinding_;
	TimeIntegrator timeIntegrator_;
	bool useLTS_;
	UInt growSmallerTiers_;
	UInt maxNumberOfTiers_;
	bool useMaxStageSizeForLTS_;
	bool PMLConstantConductivityProfile_;
	Real PMLConductivity_;

	void printHelp() const;
	void initDefaults_();

};
#endif /* ARGUMENTS_H_ */
