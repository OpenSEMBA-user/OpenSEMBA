/*
 * OutputRequestFarField.h
 *
 *  Created on: Mar 5, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTRQ_OUTRQFARFIELD_H_
#define SRC_COMMON_INPUTS_OUTRQ_OUTRQFARFIELD_H_

#include "../outRq/OutRq.h"

class OutRqFarField : public OutRq, public ElementsGroup<Volume<> > {
public:
    OutRqFarField(
            const ElementsGroup<Volume<> >& box,
            const Domain& domain,
            const string& name,
            const Real iTh, const Real fTh, const Real sTh,
            const Real iPhi, const Real fPhi, const Real sPhi);
    virtual ~OutRqFarField();
    Real getStepPhi() const;
    Real getStepTheta() const;
    Real getFinalPhi() const;
    Real getFinalTheta() const;
    Real getInitialPhi() const;
    Real getInitialTheta() const;
    bool isSimilar(const OutRqFarField& rhs) const;
    void setElements(const ElementsGroup<VolR>&); // TODO VolR->Vol
    void setElements(const ElementsGroup<VolI>&); // TODO VolR->Vol
private:
    Real initialTheta_, finalTheta_, stepTheta_;
    Real initialPhi_, finalPhi_, stepPhi_;
    void
    setThetaAndPhi(
            Real iTh, Real fTh, Real sTh,
            Real iPhi, Real fPhi, Real sPhi);
};

#endif /* SRC_COMMON_INPUTS_OUTRQ_OUTRQFARFIELD_H_ */
