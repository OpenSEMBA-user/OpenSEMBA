// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
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

#include "Waveform.h"

#include <cmath>
#include <iostream>

namespace SEMBA {
    namespace Math {
        namespace Function {
            Waveform::Waveform(Type type) {
                if (type == Type::A) {
                    I0_    =   218810.0;
                    alpha_ =    11354.0;
                    beta_  =   647265.0;
                    gamma_ =  5423540.0;
                } else if (type == Type::D) {
                    I0_    =   109405.0;
                    alpha_ =    22708.0;
                    beta_  =  1294530.0;
                    gamma_ = 10847100.0;
                } else if (type == Type::H) {
                    I0_    =    10572.0;
                    alpha_ =   187191.0;
                    beta_  = 19105100.0;
                    gamma_ = 15330600.0;
                } else {
                    throw std::logic_error("Invalid Waveform type");
                }
            }

            Real Waveform::operator ()(const Real& t) const {
                return I0_ * 
                    (std::exp(-alpha_*t) - std::exp(-beta_*t)) *
                    std::pow((1 - std::exp(-gamma_*t)), 2);
            }

            bool Waveform::operator==(const Base& rhs) const {
                if (typeid(*this) != typeid(rhs)) {
                    return false;
                }
                const Waveform* rhsPtr = dynamic_cast<const Waveform*>(&rhs);
                return this->I0_ == rhsPtr->I0_ &&
                    this->alpha_ == rhsPtr->alpha_ &&
                    this->beta_ == rhsPtr->beta_ &&
                    this->gamma_ == rhsPtr->gamma_;
            }

            void Waveform::printInfo() const {
                std::cout << " --- Function Waveform info --- " << std::endl;
                FunctionRR::printInfo();
            }

        } /* namespace Function */
    } /* namespace Math */
} /* namespace SEMBA */
