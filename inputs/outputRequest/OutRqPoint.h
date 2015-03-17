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
};

#endif /* SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQPOINT_H_ */
