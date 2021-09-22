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

#include "math/Types.h"
#include "geometry/Box.h"

#include "class/Class.h"
#include "class/Cloneable.h"
#include "class/Shareable.h"
#include "class/Printable.h"

#include "geometry/element/Element.h"
#include "geometry/element/Group.h"

namespace SEMBA {
namespace Geometry {
namespace Mesh {

class Mesh : public virtual Class::Class,
             public virtual Class::Cloneable,
             public virtual Class::Shareable,
             public virtual Class::Printable {
public:
    Mesh();
    virtual ~Mesh();

    virtual void applyScalingFactor(const Math::Real factor) = 0;
    virtual BoxR3 getBoundingBox() const = 0;
    virtual void reassignPointers(
            const SEMBA::Group::Identifiable<Element::Model, MatId>& =
                SEMBA::Group::Identifiable<Element::Model, MatId>()) = 0;

    virtual void reassign( Element::Group<const Elem>& ) = 0;
};

} /* namespace Mesh */
} /* namespace Geometry */
} /* namespace SEMBA */

