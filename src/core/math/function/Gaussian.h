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

#ifndef SRC_COMMON_MATH_FUNCTIONGAUSSIAN_H_
#define SRC_COMMON_MATH_FUNCTIONGAUSSIAN_H_

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

#endif /* SRC_COMMON_MATH_FUNCTIONGAUSSIAN_H_ */
