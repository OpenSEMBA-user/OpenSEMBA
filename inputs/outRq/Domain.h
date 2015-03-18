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
    typedef enum {
        NONE, TIME, FREQ, TRAN, TIFR, TITR, FRTR, ALL
    } Type;
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

    Domain& operator=(const Domain& rhs);

    bool operator==(const Domain& rhs) const;

    Real getSamplingPeriod() const;
    Real getFinalTime() const;
    Real getInitialTime() const;
    Real getFinalFrequency() const;
    bool isFrequencyDomain() const;
    Real getFrequencyStep() const;
    Real getInitialFrequency() const;
    bool isLogFrequencySweep() const;
    bool isTimeDomain() const;
    const string& getTransferFunctionFile() const;
    bool isUsingTransferFunction() const;

    Type getDomainType() const;

    void printInfo() const;
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
