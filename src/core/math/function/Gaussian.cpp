

#include "Gaussian.h"

#include <cmath>
#include <iostream>

namespace SEMBA {
namespace Math {
namespace Function {

Real Gaussian::operator ()(const Real& time) const {
    static const Real pi    = Constants::pi;
    static const Real sqrt2 = sqrt(2.0);
    Real expArg = (time - delay_) / (spread_ * sqrt2);

    return amplitude_ * exp(-expArg * expArg) * cos(freq_ * pi * 2.0 * time);
}

bool Gaussian::operator==(const Base& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    const Gaussian* rhsPtr = dynamic_cast<const Gaussian*>(&rhs);
    return ((this->getSpread()    == rhsPtr->getSpread()) &&
            (this->getDelay()     == rhsPtr->getDelay()) &&
            (this->getAmplitude() == rhsPtr->getAmplitude()) &&
            (this->getFreq()      == rhsPtr->getFreq()));
}

std::complex<Real> Gaussian::getFourier(const Real freq) const {
    const Real pi2 = (Real) 2.0 * Constants::pi;
    const std::complex<Real> phase(0.0, getDelay() *pi2*freq);
    const std::complex<Real> phaseShift = std::exp(phase);
    return getAmplitude() * getSpread() * std::sqrt(pi2)
            * phaseShift
            * std::exp(- (Real) 0.5 * pow(getSpread()*pi2*freq,2));
}

void Gaussian::printInfo() const {
    std::cout << " --- Function Gaussian info --- " << std::endl;
    FunctionRR::printInfo();
    std::cout << "Ampliude:  " << amplitude_ << std::endl;
    std::cout << "Spread:    " << spread_ << std::endl;
    std::cout << "Delay:     " << delay_ << std::endl;
    std::cout << "Frequency: " << freq_ << std::endl;
}

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */
