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

#ifndef SEMBA_OUTPUTREQUEST_FARFIELD_H_
#define SEMBA_OUTPUTREQUEST_FARFIELD_H_

#include "OutputRequest.h"

namespace SEMBA {
namespace OutputRequest {

class FarField : public virtual OutputRequest<Geometry::Vol> {
public:
    FarField(const Domain& domain,
             const std::string& name,
             const Geometry::Element::Group<Geometry::Vol>& box,
             const Math::Real iTh, const Math::Real fTh, const Math::Real sTh,
             const Math::Real iPhi, const Math::Real fPhi,
             const Math::Real sPhi);
    FarField(const FarField& rhs);
    ~FarField();

    SEMBA_CLASS_DEFINE_CLONE(FarField);

    bool hasSameProperties(const SEMBA::OutputRequest::Base& rhs) const;

    Math::Real getInitialTheta() const;
    Math::Real getFinalTheta() const;
    Math::Real getStepTheta() const;
    Math::Real getInitialPhi() const;
    Math::Real getFinalPhi() const;
    Math::Real getStepPhi() const;

    void printInfo() const;
private:
    Math::Real initialTheta_, finalTheta_, stepTheta_;
    Math::Real initialPhi_, finalPhi_, stepPhi_;

    void setThetaAndPhi(Math::Real iTh, Math::Real fTh, Math::Real sTh,
                        Math::Real iPhi, Math::Real fPhi, Math::Real sPhi);
};

} /* namespace OutputRequest */
} /* namespace SEMBA */

#endif /* SEMBA_OUTPUTREQUEST_FARFIELD_H_ */
