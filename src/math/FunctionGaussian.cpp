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
 * Gaussian.cpp
 *
 *  Created on: Sep 9, 2015
 *      Author: luis
 */

#include "FunctionGaussian.h"


FunctionGaussian::FunctionGaussian() {
    spread_ = 0.0;
    delay_  = 0.0;
    freq_   = 0.0;
}

FunctionGaussian::FunctionGaussian(const Real spread,
                                     const Real delay,
                                     const Real freq) {
    spread_ = spread;
    delay_  = delay;
    freq_   = freq;
}

FunctionGaussian::FunctionGaussian(const FunctionGaussian& rhs) {
    spread_ = rhs.spread_;
    delay_  = rhs.delay_;
    freq_   = rhs.freq_;
}

FunctionGaussian::~FunctionGaussian() {

}

Real FunctionGaussian::operator ()(const Real& time) const {
    static const Real pi    = acos(-1.0);
    static const Real sqrt2 = sqrt(2.0);
    Real expArg = (time - delay_) / (spread_ * sqrt2);

    return exp(-expArg * expArg) * cos(freq_ * pi * 2.0 * time);
}

bool FunctionGaussian::operator==(const FunctionBase& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    const FunctionGaussian* rhsPtr = rhs.castTo<FunctionGaussian>();
    return ((this->getSpread() == rhsPtr->getSpread()) &&
            (this->getDelay()  == rhsPtr->getDelay())  &&
            (this->getFreq()   == rhsPtr->getFreq()));
}

Real FunctionGaussian::getSpread() const {
   return spread_;
}

Real FunctionGaussian::getDelay() const {
   return delay_;
}

Real FunctionGaussian::getFreq() const {
   return freq_;
}

void FunctionGaussian::printInfo() const {
    cout << " --- Function Gaussian info --- " << endl;
    FunctionRR::printInfo();
    cout << "Spread:    " << spread_ << endl;
    cout << "Delay:     " << delay_ << endl;
    cout << "Frequency: " << freq_ << endl;
}
