/*
 * OutputRequestFarField.cpp
 *
 *  Created on: Mar 5, 2015
 *      Author: luis
 */

#include "OutRqFarField.h"

OutRqFarField::~OutRqFarField() {

}

OutRqFarField::OutRqFarField(
 const Domain& domain,
 const ElementBase::Type elementType,
 const string& name,
 const vector<ElementId>& elem,
 const Real iTh, const Real fTh, const Real sTh,
 const Real iPhi, const Real fPhi, const Real sPhi)
: OutRq(domain, elementType, farField, name, elem) {
    setThetaAndPhi(iTh, fTh, sTh, iPhi, fPhi, sPhi);
}

Real OutRqFarField::getStepPhi() const {
    return stepPhi_;
}

Real OutRqFarField::getStepTheta() const {
    return stepTheta_;
}

Real OutRqFarField::getFinalPhi() const {
    return finalPhi_;
}

Real OutRqFarField::getFinalTheta() const {
    return finalTheta_;
}

Real OutRqFarField::getInitialPhi() const {
    return initialPhi_;
}

Real OutRqFarField::getInitialTheta() const {
    return initialTheta_;
}

bool OutRqFarField::isSimilar(const OutRqFarField& rhs) const {
    bool isSimilar = OutRq::isSimilar(rhs);
    isSimilar &= initialTheta_ == rhs.initialTheta_;
    isSimilar &= finalTheta_ == rhs.finalTheta_;
    isSimilar &= stepTheta_ == rhs.stepTheta_;
    isSimilar &= initialPhi_ == rhs.initialPhi_;
    isSimilar &= finalPhi_ == rhs.finalPhi_;
    isSimilar &= stepPhi_ == rhs.stepPhi_;
    return isSimilar;
}

void OutRqFarField::setThetaAndPhi(
        Real iTh, Real fTh, Real sTh,
        Real iPhi, Real fPhi, Real sPhi) {
    initialTheta_ = iTh;
    finalTheta_ = fTh;
    stepTheta_ = sTh;
    initialPhi_ = iPhi;
    finalPhi_ = fPhi;
    stepPhi_ = sPhi;
}
