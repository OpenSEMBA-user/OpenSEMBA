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

#include "BoundTerminations.h"

namespace SEMBA {
namespace Geometry {

template <std::size_t D>
BoundTerminations<D>::BoundTerminations() {

}

template <std::size_t D>
BoundTerminations<D>::BoundTerminations(
    const std::array<std::array<const Bound*, 2>, D>& bounds) {
    setBounds(bounds);
}

template <std::size_t D>
BoundTerminations<D>::BoundTerminations(const BoundTerminations& rhs) {
    bounds_ = rhs.bounds_;
}

template <std::size_t D>
BoundTerminations<D>::~BoundTerminations() {

}

template <std::size_t D>
BoundTerminations<D>& BoundTerminations<D>::operator=(
        const BoundTerminations& rhs) {
    if (this == &rhs) {
        return *this;
    }
    bounds_ = rhs.bounds_;
    return *this;
}

template <std::size_t D>
const Bound* BoundTerminations<D>::operator()(std::size_t d,
                                              std::size_t p) const {
    if (bounds_[d][p]) {
        return bounds_[d][p].get();
    }
    return nullptr;
}

template <std::size_t D>
void BoundTerminations<D>::setBound(std::size_t d, std::size_t p,
                                    const Bound* bound) {
    if (bound != nullptr) {
        bounds_[d][p] =
            std::dynamic_pointer_cast<const Bound>(bound->getSharedPtr());
    } else {
        bounds_[d][p].reset();
    }
}

template <std::size_t D>
void BoundTerminations<D>::setBounds(
        const std::array<std::array<const Bound*, 2>, D>& bounds) {
    for (std::size_t d = 0; d < D; d++) {
        for (std::size_t p = 0; p < 2; p++) {
            if (bounds[d][p] != nullptr) {
                bounds_[d][p] = bounds->getSharedPtr();
            }
        }
    }
}

} /* namespace Geometry */
} /* namespace SEMBA */
