

#pragma once

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

