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

#include "BandLimited.h"

#include <cmath>
#include <iostream>

namespace SEMBA {
    namespace Math {
        namespace Function {
            BandLimited::BandLimited(
                const Real mininumFrequency, 
                const Real maximumFrequency) : 
                minimumFrequency_(mininumFrequency), 
                maximumFrequency_(maximumFrequency) {
                if (mininumFrequency <= 0.0 || maximumFrequency <= 0.0) {
                    throw std::logic_error(
                        "Band limited signals must have positive frequencies"
                    );
                }
                if (mininumFrequency >= maximumFrequency) {
                    throw std::logic_error(
                        "In band limited signals, min. frequency must be smaller max. frequency"
                    );
                }
            }

            Real BandLimited::operator ()(const Real& t) const {
                static const Real pi = Constants::pi;
                static const Real sqrt2 = sqrt(2.0);
                
                const Real carrier =
                    (maximumFrequency_ + minimumFrequency_) / 2.0;
                const Real bTuned = 
                    (maximumFrequency_ - minimumFrequency_) / 2.0;
                const Real delay = 20.0 / minimumFrequency_;
                const Real spread = 10.0 / maximumFrequency_;
                Real tD = t - delay;
                if (tD == 0.0) {
                    tD += std::numeric_limits<Real>::epsilon();
                }
                
                Real res = std::sin(2.0 * pi* bTuned * tD) / (2 * pi*bTuned*tD) *
                    exp(- std::pow(tD,2) / std::pow(spread,2) / 2.0) *
                        cos(2.0 * pi * carrier * tD);
                return res;
            }

            bool BandLimited::operator==(const Base& rhs) const {
                if (typeid(*this) != typeid(rhs)) {
                    return false;
                }
                const BandLimited* rhsPtr = dynamic_cast<const BandLimited*>(&rhs);
                return ((this->maximumFrequency_ == rhsPtr->maximumFrequency_) &&
                    (this->minimumFrequency_ == rhsPtr->minimumFrequency_)
                );
            }

            void BandLimited::printInfo() const {
                std::cout << " --- Function BandLimited info --- " << std::endl;
                FunctionRR::printInfo();
                std::cout << "Minimum frequency: " <<
                    minimumFrequency_ << std::endl;
                std::cout << "Maximum frequency: " << 
                    maximumFrequency_<< std::endl;
            }

        } /* namespace Function */
    } /* namespace Math */
} /* namespace SEMBA */
