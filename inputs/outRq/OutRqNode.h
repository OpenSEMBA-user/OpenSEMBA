/*
 * OutRqPoint.h
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTRQ_OUTRQNODE_H_
#define SRC_COMMON_INPUTS_OUTRQ_OUTRQNODE_H_

#include "OutRq.h"

class OutRqNode: public OutRq, public ElementsGroup<Node<> > {
public:
    OutRqNode(
            const ElementsGroup<Node<> >& point,
            const Domain& domain,
            const OutRq::Type& type,
            const string& name);

    virtual ClassBase* clone() const;
    void setElements(const ElementsGroup<Node<> >&);
};

#endif /* SRC_COMMON_INPUTS_OUTRQ_OUTRQNODE_H_ */
