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

#include "LinearInterpolation.h"

#include <fstream>

namespace SEMBA {
namespace Math {
namespace Function {

template<class S, class T>
inline LinearInterpolation<S, T>::LinearInterpolation() {
}

template<class S, class T>
inline LinearInterpolation<S, T>::LinearInterpolation(
        const std::vector<std::pair<S, T> >& xy) {
    for (std::size_t i = 0; i < xy.size(); i++) {
        value_.insert(xy[i]);
    }
}

template<class S, class T>
inline LinearInterpolation<S, T>::LinearInterpolation(
        const std::string& file) {
    initFromFile(file);
}

template<class S, class T>
inline LinearInterpolation<S, T>::~LinearInterpolation() {
}

template<class S, class T>
inline void LinearInterpolation<S, T>::printInfo() const {
    Function<S,T>::printInfo();
}

template<class S, class T>
inline T LinearInterpolation<S, T>::operator ()(const S& pos) const {
    if (value_.empty()) {
        throw std::out_of_range("Attempting to interpolate with no data.");
    }
    std::pair<typename std::map<S,T>::const_iterator,
              typename std::map<S,T>::const_iterator> range;
    range = value_.equal_range(pos);
    if (range.first == range.second) {
        if (range.first == value_.end()) {
            return value_.rbegin()->second;
        } else {
            if (pos <= range.first->first && range.first == value_.begin()) {
                return range.first->second;
            } else {
                typename std::map<S,T>::const_iterator prev = range.first;
                --prev;
                const Real x1 = prev->first;
                const Real y1 = prev->second;
                const Real x2 = range.second->first;
                const Real y2 = range.second->second;
                const Real m = (y2 - y1) / (x2 - x1);
                const Real n = (x2*y1 - x1*y2)/(x2 - x1);
                return (m * pos + n);
            }
        }
    } else {
        if (range.first->first == pos) {
            return range.first->second;
        } else {
            return range.first->second;
        }
    }
}

template<class S, class T>
inline bool LinearInterpolation<S, T>::operator ==(
        const Base& rhs) const {
    if (typeid(*this) != typeid(rhs)) {
        return false;
    }
    const LinearInterpolation<S,T>* rhsPtr =
        dynamic_cast<const LinearInterpolation<S,T>*>(&rhs);
    return value_ == rhsPtr->value_;
}

template<class S, class T>
void LinearInterpolation<S, T>::initFromFile(
        const std::string& file) {
    static_assert(std::is_same<S, Real>::value, "S must be Real");
    static_assert(std::is_same<T, Real>::value, "T must be Real");
    std::ifstream iStream;
    iStream.open(file, std::ifstream::in);
    if (iStream.fail()) {
        throw std::ios_base::failure(std::string("File: ") + file +
                                     std::string(" not exists"));
    }

    while (!iStream.eof()) {
        std::pair<Real, Real> value;
        iStream >> value.first >> value.second;
        value_.insert(value);
    }

    if (value_.size() == 0) {
        throw std::ios_base::failure(std::string("No data in file: ") + file);
    }
}

} /* namespace Function */
} /* namespace Math */
} /* namespace SEMBA */
