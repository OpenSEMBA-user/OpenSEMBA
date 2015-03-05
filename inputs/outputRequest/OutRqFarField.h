/*
 * OutputRequestFarField.h
 *
 *  Created on: Mar 5, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQFARFIELD_H_
#define SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQFARFIELD_H_

#include "OutputRequest.h"

class OutRqFarField : public OutputRequest {
public:
    virtual ~OutRqFarField();
    OutRqFarField(
            const Domain& domain,
            const Element::Type elementType,
            const string& name_,
            const BoxD3& box,
            const double iTh, const double fTh, const double sTh,
            const double iPhi, const double fPhi, const double sPhi);
    double getStepPhi() const;
    double getStepTheta() const;
    double getFinalPhi() const;
    double getFinalTheta() const;
    double getInitialPhi() const;
    double getInitialTheta() const;
    bool isSimilar(const OutRqFarField& rhs) const;
private:
    double initialTheta_, finalTheta_, stepTheta_;
    double initialPhi_, finalPhi_, stepPhi_;
    void
    setThetaAndPhi(
            double iTh, double fTh, double sTh,
            double iPhi, double fPhi, double sPhi);
};

#endif /* SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQFARFIELD_H_ */
