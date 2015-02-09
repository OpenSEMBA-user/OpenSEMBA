/*
 * Domain.h
 *
 *  Created on: Oct 9, 2014
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_DOMAIN_H_
#define SRC_COMMON_INPUTS_DOMAIN_H_

#include <string>
#include <iostream>

using namespace std;

class Domain {
public:
	Domain();
	Domain(
	 bool timeDomain,
	 double initialTime,
	 double finalTime,
	 double samplingPeriod,
	 bool frequencyDomain,
 	 double initialFrequency,
	 double finalFrequency,
	 double frequencyStep,
	 bool logFrequencySweep,
	 bool usingTransferFunction,
	 string transferFunctionFile);

	virtual ~Domain();

	Domain&
	operator=(const Domain& rhs);
	bool
	operator==(const Domain& rhs) const;

	double
	getSamplingPeriod() const {
		return samplingPeriod_;
	}

	double
	getFinalTime() const {
		return finalTime_;
	}

	double
	getInitialTime() const {
		return initialTime_;
	}

	void
	printInfo() const;

	double getFinalFrequency() const {
		return finalFrequency_;
	}

	bool isFrequencyDomain() const {
		return frequencyDomain_;
	}

	double getFrequencyStep() const {
		return frequencyStep_;
	}

	double getInitialFrequency() const {
		return initialFrequency_;
	}

	bool isLogFrequencySweep() const {
		return logFrequencySweep_;
	}

	bool isTimeDomain() const {
		return timeDomain_;
	}

	const string& getTransferFunctionFile() const {
		return transferFunctionFile_;
	}

	bool isUsingTransferFunction() const {
		return usingTransferFunction_;
	}

private:
	bool timeDomain_;
	double initialTime_;
	double finalTime_;
	double samplingPeriod_;
	bool frequencyDomain_;
	double initialFrequency_;
	double finalFrequency_;
	double frequencyStep_;
	bool logFrequencySweep_;
	bool usingTransferFunction_;
	string transferFunctionFile_;
};

#endif /* SRC_COMMON_INPUTS_DOMAIN_H_ */
