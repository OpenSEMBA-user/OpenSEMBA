/*
 * OutRqBulkCurrent.h
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTRQ_OUTRQSURFACE_H_
#define SRC_COMMON_INPUTS_OUTRQ_OUTRQSURFACE_H_

#include "../outRq/OutRq.h"

class OutRqSurface: public OutRq, public ElementsGroup<Surface<> > {
public:
    OutRqSurface(
            const ElementsGroup<Surface<> >& elems,
            const Domain& domain,
            const OutRq::Type& type,
            const string& name);

    virtual ClassBase* clone() const;
    void setElements(const ElementsGroup<Surface<> >&);
};

#endif /* SRC_COMMON_INPUTS_OUTRQ_OUTRQSURFACE_H_ */
