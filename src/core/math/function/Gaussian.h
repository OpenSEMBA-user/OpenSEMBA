

#pragma once

#include <complex>

#include "Function.h"

namespace SEMBA {
namespace Math {
namespace Function {

class Gaussian : public Function<Real,Real> {
public:
    Gaussian(Real spread, Real delay, Real amplitude, Real freq = 0.0) :
        spread_(spread), delay_(delay), amplitude_(amplitude), freq_(freq) {};
    
    static Gaussian buildFromMaximumFrequency(
        Real maximumFrequency, Real amplitude);

    SEMBA_MATH_FUNCTION_DEFINE_CLONE(Gaussian);

    virtual Real operator()(const Real&) const;
    bool operator==(const Base& rhs) const;
    std::complex<Real> getFourier(const Real frequency) const;

    Real getSpread() const { return spread_; }
    Real getDelay() const { return delay_; }
    Real getAmplitude() const { return amplitude_; }
    Real getFreq() const { return freq_; }


    void printInfo() const;

private:
    Real spread_;
    Real delay_;
    Real amplitude_;
    Real freq_;
};

inline Gaussian Gaussian::buildFromMaximumFrequency(
    Real maximumFrequency, Real amplitude) {
    if (maximumFrequency <= 0.0) {
        throw std::logic_error(
            "Maximum frequency must be a positive real number."
        );
    }
    Real spread = 0.2 / maximumFrequency;
    Real delay = 6.0 * spread;
    return Gaussian(spread, delay, amplitude);
}

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */

