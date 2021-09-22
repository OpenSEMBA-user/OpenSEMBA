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

#include "Element.h"

namespace SEMBA {
namespace Geometry {
namespace Element {

class SurfaceBase : public virtual Base {
public:
    SurfaceBase() {};
    virtual ~SurfaceBase() {};
};

template<class T>
class Surface : public virtual Element<T>,
                public virtual SurfaceBase {
public:
    Surface();
    virtual ~Surface();

    bool isRectangular() const;
    bool isContainedInPlane() const;
    bool isContainedInPlane(const Math::Constants::CartesianPlane plane) const;

    virtual Math::Vector::Cartesian<T,3> getNormal() const;

    virtual void printInfo() const;
};

} /* namespace Element */

typedef Element::SurfaceBase         Surf;
typedef Element::Surface<Math::Real> SurfR;
typedef Element::Surface<Math::Int > SurfI;

} /* namespace Geometry */
} /* namespace SEMBA */

#include "Surface.hpp"

