#pragma once

#include "OutputRequest.h"

namespace SEMBA {
namespace OutputRequest {

class FarField : public virtual OutputRequest {
public:
    FarField(const Domain& domain,
             const std::string& name,
             const Target& box,
             const Math::Real iTh, const Math::Real fTh, const Math::Real sTh,
             const Math::Real iPhi, const Math::Real fPhi,
             const Math::Real sPhi);
    virtual ~FarField() = default;

    std::unique_ptr<OutputRequest> clone() const override {
        return std::make_unique<FarField>(*this);
    }

    Math::Real initialTheta, finalTheta, stepTheta;
    Math::Real initialPhi, finalPhi, stepPhi;
};

} /* namespace OutputRequest */
} /* namespace SEMBA */

