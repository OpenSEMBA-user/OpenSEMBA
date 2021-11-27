#include "FarField.h"

namespace SEMBA {
namespace OutputRequest {

FarField::FarField(const Domain& domain,
                   const std::string& name,
                   const Target& elem,
                   const Math::Real iTh, const Math::Real fTh, const Math::Real sTh,
                   const Math::Real iPhi, const Math::Real fPhi, const Math::Real sPhi)
:   OutputRequest(Type::electricFarField, name, domain, elem) 
{
    initialTheta = iTh;
    finalTheta = fTh;
    stepTheta = sTh;
    initialPhi = iPhi;
    finalPhi = fPhi;
    stepPhi = sPhi;
}

} /* namespace OutputRequest */
} /* namespace SEMBA */
