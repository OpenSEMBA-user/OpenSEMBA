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
 const Element::Type elementType,
 const string& name,
 const BoxD3& box,
 const double iTh, const double fTh, const double sTh,
 const double iPhi, const double fPhi, const double sPhi)
: OutRq(domain, elementType, farField, name, box) {
    setThetaAndPhi(iTh, fTh, sTh, iPhi, fPhi, sPhi);
}

inline double OutRqFarField::getStepPhi() const {
    return stepPhi_;
}

inline double OutRqFarField::getStepTheta() const {
    return stepTheta_;
}

inline double OutRqFarField::getFinalPhi() const {
    return finalPhi_;
}

inline double OutRqFarField::getFinalTheta() const {
    return finalTheta_;
}

inline double OutRqFarField::getInitialPhi() const {
    return initialPhi_;
}

inline double OutRqFarField::getInitialTheta() const {
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
        double iTh, double fTh, double sTh,
        double iPhi, double fPhi, double sPhi) {
    initialTheta_ = iTh;
    finalTheta_ = fTh;
    stepTheta_ = sTh;
    initialPhi_ = iPhi;
    finalPhi_ = fPhi;
    stepPhi_ = sPhi;
}
