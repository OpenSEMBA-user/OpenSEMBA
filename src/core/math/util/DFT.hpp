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

#include "DFT.h"

namespace SEMBA {
namespace Math {
namespace Util {

template<class T>
std::complex<T> getDFT(const Real frequency,
                       const std::vector<T>& time,
                       const std::vector<T>& signal) {
    const std::size_t N = signal.size();
    const std::complex<T> constPart(0.0, -(T)(2.0 * Constants::pi*frequency));
    std::complex<T> res(0.0, 0.0);
    for (std::size_t i = 0; i < N; i++) {
        res += signal[i] * exp(constPart * time[i]);
    }
    res /= (Real) N;
    return conj(res);
}

template<class T, std::size_t D>
Vector::Cartesian<std::complex<T>,D> getDFT(
        const Real frequency,
        const std::vector<T>& time,
        const std::vector<Vector::Cartesian<T,D> >& signal) {
    Vector::Cartesian<std::complex<T>,D> res;
    const std::size_t N = signal.size();
    std::vector<T> auxSignal(N);
    for (std::size_t d = 0; d < D; d++) {
        for (std::size_t i = 0; i < N; i++) {
            auxSignal[i] = signal[i](d);
        }
        res(d) = getDFT(frequency, time, auxSignal);
    }
    return res;
}

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */
