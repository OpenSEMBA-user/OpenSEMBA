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

#pragma once

#include <array>
#include <memory>
#include "geometry/element/Element.h"

namespace SEMBA {
namespace Geometry {

typedef Element::Model Bound;

template <std::size_t D>
class BoundTerminations {
public:
    BoundTerminations();
    BoundTerminations(const std::array<std::array<const Bound*, 2>, D>&);
    BoundTerminations(const BoundTerminations&);
    virtual ~BoundTerminations();

    BoundTerminations& operator=(const BoundTerminations&);

    const Bound* operator()(std::size_t d, std::size_t p) const;

    void setBound (std::size_t d, std::size_t p, const Bound*);
    void setBounds(const std::array<std::array<const Bound*, 2>, D>&);

private:
    std::array<std::array<std::shared_ptr<const Bound>, 2>, D> bounds_;
};

typedef BoundTerminations<3> BoundTerminations3;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "BoundTerminations.hpp"

