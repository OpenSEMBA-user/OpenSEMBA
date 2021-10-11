

#pragma once

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

    Math::Real getInitialTheta() const;
    Math::Real getFinalTheta() const;
    Math::Real getStepTheta() const;
    Math::Real getInitialPhi() const;
    Math::Real getFinalPhi() const;
    Math::Real getStepPhi() const;

private:
    Math::Real initialTheta_, finalTheta_, stepTheta_;
    Math::Real initialPhi_, finalPhi_, stepPhi_;

    void setThetaAndPhi(Math::Real iTh, Math::Real fTh, Math::Real sTh,
                        Math::Real iPhi, Math::Real fPhi, Math::Real sPhi);
};

} /* namespace OutputRequest */
} /* namespace SEMBA */

