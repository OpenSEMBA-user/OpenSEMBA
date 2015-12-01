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

#include "Statistic.h"

namespace SEMBA {
namespace Math {
namespace Util {

template<class T>
T meanDifference(const std::vector<T>& vec) {
    if (vec.size() == 1) {
        return (T) 0.0;
    }
    const Size vS = vec.size();
    std::vector<T> diff(vS-1, 0.0);
    for (Size i = 1; i < vS; i++) {
        diff[i-1] = vec[i] - vec[i-1];
    }
    return mean(diff);
}

template<class T>
T mean(const std::vector<T>& vec) {
    const Size vS = vec.size();
    return sum(vec) / vS;
}

template<class T>
T sum(const std::vector<T>& vec) {
    const Size vS = vec.size();
    T res = 0.0;
    for (Size i = 0; i < vS; i++) {
        res += vec[i];
    }
    return res;
}

} /* namespace Util */
} /* namespace Math */
} /* namespace SEMBA */
