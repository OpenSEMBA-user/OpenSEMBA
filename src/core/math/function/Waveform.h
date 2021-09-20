// OpenSEMBA
// Copyright (C) 2019 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nu�ez (miguel@semba.guru)
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

#ifndef SRC_COMMON_MATH_FUNCTION_WAVEFORM_H_
#define SRC_COMMON_MATH_FUNCTION_WAVEFORM_H_

#include <complex>
#include <limits>

#include "Function.h"

namespace SEMBA {
    namespace Math {
        namespace Function {

            class Waveform : public Function<Real, Real> {
            public:
                enum class Type {
                    A,
                    D,
                    H
                };
                Waveform(Type type);
                
                SEMBA_MATH_FUNCTION_DEFINE_CLONE(Waveform);

                Real operator()(const Real& t) const;
                bool operator==(const Base& rhs) const;

                void printInfo() const;
            private:
                Real I0_;
                Real alpha_;
                Real beta_;
                Real gamma_;
            };

        } /* namespace Function */
    } /* namespace Math */
} /* namespace SEMBA */

#endif /* SRC_COMMON_MATH_FUNCTION_WAVEFORM_H_ */
