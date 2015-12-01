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
    Domain(bool timeDomain,
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
    Domain(const Domain& rhs);
    virtual ~Domain();

    Domain& operator=(const Domain& rhs);
    void setFinalTime(const Real finalTime);
    void setSamplingPeriod(const Real samplingPeriod);


    bool operator==(const Domain& rhs) const;

    bool isTimeDomain() const;
    Real getInitialTime() const;
    Real getFinalTime() const;
    Real getSamplingPeriod() const;
    bool isFrequencyDomain() const;
    Real getInitialFrequency() const;
    Real getFinalFrequency() const;
    Real getFrequencyStep() const;
    bool isLogFrequencySweep() const;

    bool isUsingTransferFunction() const;
    const string& getTransferFunctionFile() const;

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
