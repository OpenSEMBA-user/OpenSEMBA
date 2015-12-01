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

Gaussian::Gaussian() {
    spread_ = 0.0;
    delay_  = 0.0;
    freq_   = 0.0;
}

Gaussian::Gaussian(const Real spread,
                   const Real delay,
                   const Real freq) {
    spread_ = spread;
    delay_  = delay;
    freq_   = freq;
}

Gaussian::Gaussian(const Gaussian& rhs) {
    spread_ = rhs.spread_;
    delay_  = rhs.delay_;
    freq_   = rhs.freq_;
}

Gaussian::~Gaussian() {

}

Real Gaussian::operator ()(const Real& time) const {
    static const Real pi    = acos(-1.0);
    static const Real sqrt2 = sqrt(2.0);
    Real expArg = (time - delay_) / (spread_ * sqrt2);

    return exp(-expArg * expArg) * cos(freq_ * pi * 2.0 * time);
}

bool Gaussian::operator==(const Base& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    const Gaussian* rhsPtr = dynamic_cast<const Gaussian*>(&rhs);
    return ((this->getSpread() == rhsPtr->getSpread()) &&
            (this->getDelay()  == rhsPtr->getDelay())  &&
            (this->getFreq()   == rhsPtr->getFreq()));
}

Real Gaussian::getSpread() const {
   return spread_;
}

Real Gaussian::getDelay() const {
   return delay_;
}

Real Gaussian::getFreq() const {
   return freq_;
}

void Gaussian::printInfo() const {
    std::cout << " --- Function Gaussian info --- " << std::endl;
    FunctionRR::printInfo();
    std::cout << "Spread:    " << spread_ << std::endl;
    std::cout << "Delay:     " << delay_ << std::endl;
    std::cout << "Frequency: " << freq_ << std::endl;
}

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */
