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

    template<class T2>
    bool isSimilar(const T2* rhs, const bool rev = false) const {
        if(!rhs->is<OutRqFarField>() || !this->is<T2>()) {
            return false;
        }
        const OutRqFarField* rhsFF = rhs->castTo<OutRqFarField>();
        bool isSimilar = OutRq<Vol>::isSimilar(rhs, true);
        isSimilar &= initialTheta_ == rhsFF->initialTheta_;
        isSimilar &= finalTheta_ == rhsFF->finalTheta_;
        isSimilar &= stepTheta_ == rhsFF->stepTheta_;
        isSimilar &= initialPhi_ == rhsFF->initialPhi_;
        isSimilar &= finalPhi_ == rhsFF->finalPhi_;
        isSimilar &= stepPhi_ == rhsFF->stepPhi_;
        if (!rev) {
            isSimilar &= rhs->isSimilar(this->castTo<T2>(), true);
        }
        return isSimilar;
    }

    Real getInitialTheta() const;
    Real getFinalTheta() const;
    Real getStepTheta() const;
    Real getInitialPhi() const;
    Real getFinalPhi() const;
    Real getStepPhi() const;
private:
    Real initialTheta_, finalTheta_, stepTheta_;
    Real initialPhi_, finalPhi_, stepPhi_;

    void setThetaAndPhi(Real iTh, Real fTh, Real sTh,
                        Real iPhi, Real fPhi, Real sPhi);
};

#endif /* SRC_COMMON_INPUTS_OUTRQ_OUTRQFARFIELD_H_ */
