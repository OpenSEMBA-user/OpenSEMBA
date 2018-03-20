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

#include "TEMCoaxial.h"

namespace SEMBA {
namespace Source {
namespace Port {

TEMCoaxial::TEMCoaxial(
        Magnitude::Magnitude* magnitude,
        const Geometry::Element::Group<const Geometry::Surf>& elem,
        const ExcitationMode excMode,
        const Math::CVecR3& origin,
        const Math::Real innerRadius,
        const Math::Real outerRadius) :
        SEMBA::Source::Base(magnitude),
        Geometry::Element::Group<const Geometry::Surf>(elem),
        TEM(magnitude, elem, excMode) {
    origin_ = origin;
    innerRadius_ = innerRadius;
    outerRadius_ = outerRadius;
}

TEMCoaxial::TEMCoaxial(
        const TEMCoaxial& rhs) :
                        SEMBA::Source::Base(rhs),
                        Geometry::Element::Group<const Geometry::Surf>(rhs),
                        TEM(rhs) {
    origin_ = rhs.origin_;
    innerRadius_ = rhs.innerRadius_;
    outerRadius_ = rhs.outerRadius_;
}

TEMCoaxial::~TEMCoaxial() {
}

Math::CVecR3 TEMCoaxial::getOrigin() const {
    return origin_;
}

const std::string& TEMCoaxial::getName() const {
    const static std::string res = "Coaxial_TEM_port";
    return res;
}

Math::CVecR3 TEMCoaxial::getWeight(
        const Math::CVecR3& pos) const {
    // Return normalized weights for electric field components.
    const Math::Real rho = (pos - getOrigin()).norm();
    switch (getExcitationMode()) {
    case ExcitationMode::voltage:
    {
        const Math::CVecR3 rhoHat = (pos - getOrigin()).normalize();
        return rhoHat / (rho * log(outerRadius_/innerRadius_));
    }
    case ExcitationMode::current:
    {
        const Math::CVecR3 phiHat = (Math::CVecR3(0,0,1) ^ pos).normalize();
        return phiHat / (2.0 * Math::Constants::pi * rho);
    }
    default:
        throw std::logic_error("Unsupported excitation mode.");
    }
}

void TEMCoaxial::printInfo() const {
    std::cout << " --- Coaxial TEM Port --- " << std::endl;
    std::cout << "Origin: " << origin_ << std::endl;
    std::cout << "Inner/Outer radii: "
            << innerRadius_ << "/" << outerRadius_ << std::endl;
    TEM::printInfo();
}

void TEMCoaxial::set(
        const Geometry::Element::Group<const Geometry::Elem>& elemGroup) {
    // Reescales internal dimensions.
    Geometry::BoxR3 box = elemGroup.getBound();
    const Math::CVecR3 diagonal = box.getMax()-box.getMin();
    if (!diagonal.isContainedInPlane(Math::Constants::CartesianPlane::xy)) {
        throw std::logic_error("Port is not contained in a XY plane");
    }
    const Math::Real width  = box.getMax()(Math::Constants::x) -
                              box.getMin()(Math::Constants::x);
    const Math::Real height = box.getMax()(Math::Constants::y) -
                              box.getMin()(Math::Constants::y);
    const Math::Real averageNewRadius = (width + height)/4;
    innerRadius_ *= averageNewRadius;
    outerRadius_ *= averageNewRadius;
    const Math::CVecR3 averageNewOrigin = (box.getMax() + box.getMin()) / 2;
    origin_ = averageNewOrigin;
    //
    Source<Geometry::Surf>::set(elemGroup);
}

} /* namespace Port */
} /* namespace Source */
} /* namespace SEMBA */
