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

#include "Source.h"

namespace SEMBA {
namespace Source {

class Dipole : public Source<Geometry::Vol> {
public:
    Dipole(const Magnitude::Magnitude* magnitude,
           const Geometry::Element::Group<Geometry::Vol>& elem,
           Math::Real   length,
           Math::CVecR3 orientation,
           Math::CVecR3 position);
    Dipole(const Dipole& rhs);
    virtual ~Dipole();

    SEMBA_CLASS_DEFINE_CLONE(Dipole);

    const std::string& getName() const;
    void printInfo() const;
protected:
    Math::Real length_;
    Math::CVecR3 orientation_;
    Math::CVecR3 position_;
    Math::Real gaussDelay_;
    Math::Real spreadSqrt2_;
};

} /* namespace Source */
} /* namespace SEMBA */

