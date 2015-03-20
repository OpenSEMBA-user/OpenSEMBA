/*
 * OutputRequestFarField.h
 *
 *  Created on: Mar 5, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTRQ_OUTRQFARFIELD_H_
#define SRC_COMMON_INPUTS_OUTRQ_OUTRQFARFIELD_H_

#include "OutRq.h"

class OutRqFarField : public virtual OutRq<Vol> {
public:
    OutRqFarField(const Domain& domain,
                  const string& name,
                  const ElementsGroup<Volume<> >& box,
                  const Real iTh, const Real fTh, const Real sTh,
                  const Real iPhi, const Real fPhi, const Real sPhi);
    OutRqFarField(const OutRqFarField& rhs);
    ~OutRqFarField();

    virtual ClassBase* clone() const;

    virtual OutRqFarField& operator=(const ElementsGroup<Vol>& rhs);

    Real getInitialTheta() const;
    Real getFinalTheta() const;
    Real getStepTheta() const;
    Real getInitialPhi() const;
    Real getFinalPhi() const;
    Real getStepPhi() const;

    bool isSimilar(const OutRqFarField& rhs) const;
private:
    Real initialTheta_, finalTheta_, stepTheta_;
    Real initialPhi_, finalPhi_, stepPhi_;

    void setThetaAndPhi(Real iTh, Real fTh, Real sTh,
                        Real iPhi, Real fPhi, Real sPhi);
};

#endif /* SRC_COMMON_INPUTS_OUTRQ_OUTRQFARFIELD_H_ */
