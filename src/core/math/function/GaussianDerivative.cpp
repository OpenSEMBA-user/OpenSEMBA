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

#include "GaussianDerivative.h"

#include <cmath>
#include <iostream>


namespace SEMBA{
namespace Math{
namespace Function{

GaussianDerivative::GaussianDerivative(){
	 spread_ = 0.0;
	 delay_  = 0.0;
	 freq_   = 0.0;

}

GaussianDerivative::GaussianDerivative(const Real spread,
									   const Real delay,
									   const Real freq)
	: spread_{spread}, delay_{delay}, freq_{freq}{

}

GaussianDerivative::GaussianDerivative(const GaussianDerivative& rhs)
	: spread_{rhs.spread_}, delay_{rhs.delay_}, freq_{rhs.freq_}{

}

GaussianDerivative::~GaussianDerivative(){

}

Real GaussianDerivative::operator ()(const Real& time) const {
    static const Real pi    = Constants::pi;
    static const Real sqrt2 = sqrt(2.0);
    Real expArg = (time - delay_) / (spread_ * sqrt2);
    Real factor = -(time - delay_) / pow(spread_, 2);

    return factor * exp(-expArg * expArg) * cos(freq_ * pi * 2.0 * time);
}

bool GaussianDerivative::operator==(const Base& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    const GaussianDerivative* rhsPtr = dynamic_cast<const GaussianDerivative*>(&rhs);
    return ((this->getSpread() == rhsPtr->getSpread()) &&
            (this->getDelay()  == rhsPtr->getDelay())  &&
            (this->getFreq()   == rhsPtr->getFreq()));
}

Real GaussianDerivative::getSpread() const {
   return spread_;
}

Real GaussianDerivative::getDelay() const {
   return delay_;
}

Real GaussianDerivative::getFreq() const {
   return freq_;
}

std::complex<Real> GaussianDerivative::getFourier(const Real freq) const {
    const Real pi2 = (Real) 2.0 * Constants::pi;
    const std::complex<Real> phase(0.0, getDelay() *pi2*freq);
    const std::complex<Real> phaseShift = std::exp(phase);
    return getSpread() * std::sqrt(pi2)
            * phaseShift
            * std::exp(- (Real) 0.5 * pow(getSpread()*pi2*freq,2));
}

void GaussianDerivative::printInfo() const {
    std::cout << " --- Function Gaussian Derivative info --- " << std::endl;
    FunctionRR::printInfo();
    std::cout << "Spread:    " << spread_ << std::endl;
    std::cout << "Delay:     " << delay_ << std::endl;
    std::cout << "Frequency: " << freq_ << std::endl;
}



}/*namespace Function*/
}/*namespace Math*/
}/*namespace SEMBA*/







