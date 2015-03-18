/*
 * OutRqPoint.h
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTRQ_OUTRQPOINT_H_
#define SRC_COMMON_INPUTS_OUTRQ_OUTRQPOINT_H_

#include "../outRq/OutRq.h"

class OutRqPoint: public OutRq, public ElementsGroup<Node<> > {
public:
    OutRqPoint(
            const ElementsGroup<Node<> >& point,
            const Domain& domain,
            const OutRq::Type& type,
            const string& name);
    virtual ~OutRqPoint();

    virtual ClassBase* clone() const;
    void setElements(const ElementsGroup<Node<> >&);
};

#endif /* SRC_COMMON_INPUTS_OUTRQ_OUTRQPOINT_H_ */
