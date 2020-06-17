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
