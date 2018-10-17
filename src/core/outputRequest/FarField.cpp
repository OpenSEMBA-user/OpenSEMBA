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

#include "FarField.h"

namespace SEMBA {
namespace OutputRequest {

FarField::FarField(const Domain& domain,
                   const std::string& name,
                   const Geometry::Element::Group<Geometry::Vol>& elem,
                   const Math::Real iTh, const Math::Real fTh,
                   const Math::Real sTh,
                   const Math::Real iPhi, const Math::Real fPhi,
                   const Math::Real sPhi)
:   SEMBA::OutputRequest::Base(electricFarField, name, domain),
    Geometry::Element::Group<const Geometry::Vol>(elem) {

    setThetaAndPhi(iTh, fTh, sTh, iPhi, fPhi, sPhi);
}

FarField::FarField(const FarField& rhs)
:   SEMBA::OutputRequest::Base(rhs),
    Geometry::Element::Group<const Geometry::Vol>(rhs) {

    initialTheta_ = rhs.initialTheta_;
    finalTheta_ = rhs.finalTheta_;
    stepTheta_ = rhs.stepTheta_;
    initialPhi_ = rhs.initialPhi_;
    finalPhi_ = rhs.finalPhi_;
    stepPhi_ = rhs.stepPhi_;
}

FarField::~FarField() {

}

Math::Real FarField::getInitialTheta() const {
    return initialTheta_;
}

Math::Real FarField::getFinalTheta() const {
    return finalTheta_;
}

Math::Real FarField::getStepTheta() const {
    return stepTheta_;
}

Math::Real FarField::getInitialPhi() const {
    return initialPhi_;
}

Math::Real FarField::getFinalPhi() const {
    return finalPhi_;
}

Math::Real FarField::getStepPhi() const {
    return stepPhi_;
}

void FarField::printInfo() const {
    OutputRequest<Geometry::Vol>::printInfo();
    std::cout << "Initial theta: " << getInitialTheta() << std::endl;
    std::cout << "Final theta: " << getFinalTheta() << std::endl;
    std::cout << "Step theta: " << getStepTheta() << std::endl;
    std::cout << "Initial phi: " << getInitialPhi() << std::endl;
    std::cout << "Final phi: " << getFinalPhi() << std::endl;
    std::cout << "Step phi: " << getStepPhi() << std::endl;
}

void FarField::setThetaAndPhi(
        Math::Real iTh, Math::Real fTh, Math::Real sTh,
        Math::Real iPhi, Math::Real fPhi, Math::Real sPhi) {
    initialTheta_ = iTh;
    finalTheta_ = fTh;
    stepTheta_ = sTh;
    initialPhi_ = iPhi;
    finalPhi_ = fPhi;
    stepPhi_ = sPhi;
}

} /* namespace OutputRequest */
} /* namespace SEMBA */
