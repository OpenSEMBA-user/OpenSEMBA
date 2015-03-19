/*
 * OutRqVolume.h
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTRQ_OUTRQVOLUME_H_
#define SRC_COMMON_INPUTS_OUTRQ_OUTRQVOLUME_H_

#include "OutRq.h"

class OutRqVolume: public OutRq, public ElementsGroup<Volume<> > {
public:
    OutRqVolume(
            const ElementsGroup<Volume<> >& box,
            const Domain& domain,
            const Type& type,
            const string& name);

    virtual ClassBase* clone() const;

    void setElements(const ElementsGroup<Volume<> >&);

};

#endif /* SRC_COMMON_INPUTS_OUTRQ_OUTRQVOLUME_H_ */
