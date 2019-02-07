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

#include "Dipole.h"

namespace SEMBA {
namespace Source {

Dipole::Dipole(const Magnitude::Magnitude* magnitude,
               const Geometry::Element::Group<Geometry::Vol>& elem,
               Math::Real length,
               Math::CVecR3 orientation,
               Math::CVecR3 position)
:   SEMBA::Source::Base(magnitude),
    Geometry::Element::Group<const Geometry::Vol>(elem) {

    length_ = length;
    orientation_ = orientation;
    position_ = position;
    gaussDelay_ = 0.0;
    spreadSqrt2_ = 0.0;
//    gaussDelay_ = magnitude.getDelay();
//    spreadSqrt2_ = magnitude.getSpread() * sqrt(2.0);
}

Dipole::Dipole(const Dipole& rhs)
:   SEMBA::Source::Base(rhs),
    Geometry::Element::Group<const Geometry::Vol>(rhs) {

    length_ = rhs.length_;
    orientation_ = rhs.orientation_;
    position_ = rhs.position_;
    gaussDelay_ = rhs.gaussDelay_;
    spreadSqrt2_ = rhs.spreadSqrt2_;
}

Dipole::~Dipole() {

}

const std::string& Dipole::getName() const {
    const static std::string res = "Dipole";
    return res;
}

void Dipole::printInfo() const {
    std::cout << " ---- Dipole information ---- " << std::endl;
    SEMBA::Source::Base::printInfo();
    std::cout << " - Length: " << length_ << std::endl;
    std::cout << " - Orientation vector:";;
    orientation_.printInfo();
    std::cout << std::endl;
    std::cout << " - Position vector:";
    position_.printInfo();
    std::cout << std::endl;
}

} /* namespace Source */
} /* namespace SEMBA */
