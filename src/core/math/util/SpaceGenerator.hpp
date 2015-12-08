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

#include "SpaceGenerator.h"

namespace SEMBA {
namespace Math {
namespace Util {

template<class T>
std::vector<Real> logspace(const std::pair<Real,Real>& rangeExponents,
                           const T nPoints) {
    std::vector<Real> res;
    const Real base = (Real) 10;
    res.reserve(nPoints);
    if (nPoints == 1) {
        res.push_back(pow(base, rangeExponents.second));
    } else {
        Real jump =
        (rangeExponents.second - rangeExponents.first)
        / (Real) (nPoints - 1);
        for (std::size_t i = 0; i < nPoints; i++) {
            res.push_back(
            pow(base, rangeExponents.first + (Real) i * jump));
        }
    }
    return res;
}

template<class T>
std::vector<T> linspace(const std::pair<T,T>& range,
                        const std::size_t nPoints) {
    std::vector<T> res;
    res.reserve(nPoints);
    if (nPoints == 1) {
        res.push_back(range.second);
    } else {
        T jump = (range.second - range.first) / (T) (nPoints - 1);
        for (std::size_t i = 0; i < nPoints; i++) {
            res.push_back(range.first + (T) i * jump);
        }
    }
    return res;
}

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */
