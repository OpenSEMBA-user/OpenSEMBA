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
