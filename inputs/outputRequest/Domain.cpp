/*
 * Domain.cpp
 *
 *  Created on: Oct 9, 2014
 *      Author: luis
 */

#include "Domain.h"

Domain::Domain() {
	timeDomain_ = false;
	initialTime_ = 0.0;
	finalTime_ = 0.0;
	samplingPeriod_ = 0.0;
	frequencyDomain_ = false;
	initialFrequency_ = 0.0;
	finalFrequency_ = 0.0;
	frequencyStep_ = 0.0;
	logFrequencySweep_ = false;
	usingTransferFunction_ = false;
}

Domain::Domain(bool timeDomain, double initialTime, double finalTime,
		double samplingPeriod, bool frequencyDomain, double initialFrequency,
		double finalFrequency, double frequencyStep, bool logFrequencySweep,
		bool usingTransferFunction, string transferFunctionFile) {
	timeDomain_ = timeDomain;
	initialTime_ = initialTime;
	finalTime_ = finalTime;
	samplingPeriod_ = samplingPeriod;
	frequencyDomain_ = frequencyDomain;
	initialFrequency_ = initialFrequency;
	finalFrequency_ = finalFrequency;
	frequencyStep_ = frequencyStep;
	logFrequencySweep_ = logFrequencySweep;
	usingTransferFunction_ = usingTransferFunction;
	if (usingTransferFunction_) {
		transferFunctionFile_ = transferFunctionFile;
	}
}

Domain::~Domain() {
	// TODO Auto-generated destructor stub
}

Domain&
Domain::operator =(const Domain& rhs) {
	if (this == &rhs) {
		return *this;
	}
	timeDomain_ = rhs.timeDomain_;
	initialTime_ = rhs.initialTime_;
	finalTime_ = rhs.finalTime_;
	samplingPeriod_ = rhs.samplingPeriod_;
	frequencyDomain_ = rhs.frequencyDomain_;
	initialFrequency_ = rhs.initialFrequency_;
	finalFrequency_ = rhs.finalFrequency_;
	frequencyStep_ = rhs.frequencyStep_;
	logFrequencySweep_ = rhs.logFrequencySweep_;
	usingTransferFunction_ = rhs.usingTransferFunction_;
	transferFunctionFile_ = rhs.transferFunctionFile_;
	return *this;
}

void Domain::printInfo() const {
	cout<< "-- Domain info: " << endl;
	if (timeDomain_) {
		cout<< "Requesting time output: ";
		cout<< initialTime_ << " " << finalTime_ << " " << samplingPeriod_
				<< endl;
	}
	if (frequencyDomain_) {
		cout<< "Requesting frequency output: "
			<< initialFrequency_ << " " << finalFrequency_ << " "
			<< frequencyStep_ << endl;
		if (logFrequencySweep_) {
			cout<< "Logarithmic frequency sweep." << endl;
		} else {
			cout<< "Linear frequency sweep." << endl;
		}
		if (usingTransferFunction_) {
			cout<< "Using transfer function file: " << transferFunctionFile_
					<< endl;
		}
	}
}

bool
Domain::operator ==(const Domain& rhs) const {
	bool isSame = true;
	isSame &= timeDomain_ == rhs.timeDomain_;
	isSame &= initialTime_ == rhs.initialTime_;
	isSame &= finalTime_ == rhs.finalTime_;
	isSame &= samplingPeriod_ == rhs.samplingPeriod_;
	isSame &= frequencyDomain_ == rhs.frequencyDomain_;
	isSame &= initialFrequency_ == rhs.initialFrequency_;
	isSame &= finalFrequency_ == rhs.finalFrequency_;
	isSame &= frequencyStep_ == rhs.frequencyStep_;
	isSame &= logFrequencySweep_ == rhs.logFrequencySweep_;
	isSame &= usingTransferFunction_ == rhs.usingTransferFunction_;
	isSame &= transferFunctionFile_ == rhs.transferFunctionFile_;
	return isSame;
}
