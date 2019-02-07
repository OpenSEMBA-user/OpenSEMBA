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

#ifndef SEMBA_OUTPUTREQUEST_DOMAIN_H_
#define SEMBA_OUTPUTREQUEST_DOMAIN_H_

#include <string>

#include "math/Types.h"

namespace SEMBA {
namespace OutputRequest {

class Domain {
public:
    typedef enum {
        NONE, TIME, FREQ, TRAN, TIFR, TITR, FRTR, ALL
    } Type;

    Domain();
    Domain(bool timeDomain,
           Math::Real initialTime,
           Math::Real finalTime,
           Math::Real samplingPeriod,
           bool frequencyDomain,
           Math::Real initialFrequency,
           Math::Real finalFrequency,
           Math::Real frequencyStep,
           bool logFrequencySweep,
           bool usingTransferFunction,
           std::string transferFunctionFile);
    Domain(const Domain& rhs);
    virtual ~Domain();

    Domain& operator=(const Domain& rhs);
    void setFinalTime(const Math::Real finalTime);
    void setSamplingPeriod(const Math::Real samplingPeriod);


    bool operator==(const Domain& rhs) const;

    bool       isTimeDomain() const;
    Math::Real getInitialTime() const;
    Math::Real getFinalTime() const;
    Math::Real getSamplingPeriod() const;
    bool       isFrequencyDomain() const;
    Math::Real getInitialFrequency() const;
    Math::Real getFinalFrequency() const;
    Math::Real getFrequencyStep() const;
    bool       isLogFrequencySweep() const;

    bool isUsingTransferFunction() const;
    const std::string& getTransferFunctionFile() const;

    Type getDomainType() const;

    void printInfo() const;

private:
    bool        timeDomain_;
    Math::Real  initialTime_;
    Math::Real  finalTime_;
    Math::Real  samplingPeriod_;
    bool        frequencyDomain_;
    Math::Real  initialFrequency_;
    Math::Real  finalFrequency_;
    Math::Real  frequencyStep_;
    bool        logFrequencySweep_;
    bool        usingTransferFunction_;
    std::string transferFunctionFile_;
};

} /* namespace OutputRequest */
} /* namespace SEMBA */

#endif /* SEMBA_OUTPUTREQUEST_DOMAIN_H_ */
