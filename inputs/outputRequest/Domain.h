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

#include "Types.h"

class Domain {
public:
	Domain();
	Domain(
	 bool timeDomain,
	 Real initialTime,
	 Real finalTime,
	 Real samplingPeriod,
	 bool frequencyDomain,
 	 Real initialFrequency,
	 Real finalFrequency,
	 Real frequencyStep,
	 bool logFrequencySweep,
	 bool usingTransferFunction,
	 string transferFunctionFile);

	virtual ~Domain();

	Domain&
	operator=(const Domain& rhs);
	bool
	operator==(const Domain& rhs) const;

	Real
	getSamplingPeriod() const {
		return samplingPeriod_;
	}

	Real
	getFinalTime() const {
		return finalTime_;
	}

	Real
	getInitialTime() const {
		return initialTime_;
	}

	void
	printInfo() const;

	Real getFinalFrequency() const {
		return finalFrequency_;
	}

	bool isFrequencyDomain() const {
		return frequencyDomain_;
	}

	Real getFrequencyStep() const {
		return frequencyStep_;
	}

	Real getInitialFrequency() const {
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
	Real initialTime_;
	Real finalTime_;
	Real samplingPeriod_;
	bool frequencyDomain_;
	Real initialFrequency_;
	Real finalFrequency_;
	Real frequencyStep_;
	bool logFrequencySweep_;
	bool usingTransferFunction_;
	string transferFunctionFile_;
};

#endif /* SRC_COMMON_INPUTS_DOMAIN_H_ */
