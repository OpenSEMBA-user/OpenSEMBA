/*
 * OutRqBulkCurrent.h
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTRQ_OUTRQBULKCURRENT_H_
#define SRC_COMMON_INPUTS_OUTRQ_OUTRQBULKCURRENT_H_

#include "../outRq/OutRq.h"

class OutRqBulkCurrent: public OutRq, public ElementsGroup<Surface<> > {
public:
    OutRqBulkCurrent(
            const ElementsGroup<Surface<> >& elems,
            const Domain& domain,
            const OutRq::Type& type,
            const string& name);
    void setElements(const ElementsGroup<Surface<> >&);
};

#endif /* SRC_COMMON_INPUTS_OUTRQ_OUTRQBULKCURRENT_H_ */
