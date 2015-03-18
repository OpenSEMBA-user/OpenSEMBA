/*
 * OutRqPoint.h
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQPOINT_H_
#define SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQPOINT_H_

#include "OutRq.h"


class OutRqPoint: public OutRq, public ElementsGroup<NodeR> {
public:
    OutRqPoint();
    virtual ~OutRqPoint();
    void setElements(const ElementsGroup<NodeR>&); // TODO NodeR -> Node
    void setElements(const ElementsGroup<NodeI>&); // TODO NodeR -> Node
};

#endif /* SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQPOINT_H_ */
