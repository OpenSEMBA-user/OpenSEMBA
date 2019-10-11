// OpenSEMBA
// Copyright (C) 2019 Salvador Gonzalez Garcia        (salva@ugr.es)
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

#ifndef SRC_COMMON_MATH_FUNCTION_BANDLIMITED_H_
#define SRC_COMMON_MATH_FUNCTION_BANDLIMITED_H_

#include <complex>
#include <limits>

#include "Function.h"

namespace SEMBA {
    namespace Math {
        namespace Function {

            class BandLimited : public Function<Real, Real> {
            public:
                BandLimited(const Real minimumFrequency,
                            const Real maximumFrequency);
                
                SEMBA_MATH_FUNCTION_DEFINE_CLONE(BandLimited);

                Real operator()(const Real& t) const;
                bool operator==(const Base& rhs) const;

                void printInfo() const;
            private:
                Real minimumFrequency_;
                Real maximumFrequency_;
            };

        } /* namespace Function */
    } /* namespace Math */
} /* namespace SEMBA */

#endif /* SRC_COMMON_MATH_FUNCTION_BANDLIMITED_H_ */
