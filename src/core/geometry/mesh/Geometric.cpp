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

#include "Geometric.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

Geometric::Geometric() {

}

Geometric::Geometric(const Grid3& grid)
:   grid_(grid) {

}

Geometric::Geometric(const Grid3& grid,
                     const Coordinate::Group<const CoordR3>& cG,
                     const Element::Group<const ElemR>& elem,
                     const Layer::Group<const Layer::Layer>& layers)
:   Unstructured(cG, elem, layers),
    grid_(grid) {

}

Geometric::Geometric(const Geometric& rhs)
:   Unstructured(rhs),
    grid_(rhs.grid_) {

}

Geometric::~Geometric() {

}

Geometric& Geometric::operator=(const Geometric& rhs) {
    if(this == &rhs) {
        return *this;
    }
    Unstructured::operator=(rhs);
    grid_ = rhs.grid_;

    return *this;
}

Structured* Geometric::getMeshStructured(const Math::Real tol) const {
    return Unstructured::getMeshStructured(grid_, tol);
}

void Geometric::applyScalingFactor(const Math::Real factor) {
    Unstructured::applyScalingFactor(factor);
    grid_.applyScalingFactor(factor);
}

void Geometric::printInfo() const {
    std::cout << " --- Mesh geometric Info --- " << std::endl;
    Unstructured::printInfo();
    grid_.printInfo();
}

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */
