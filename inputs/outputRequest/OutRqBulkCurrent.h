/*
 * OutRqBulkCurrent.h
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQBULKCURRENT_H_
#define SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQBULKCURRENT_H_

#include "OutRq.h"

class OutRqBulkCurrent: public OutRq, public ElementsGroup<SurfR> {
public:
    OutRqBulkCurrent();
    virtual ~OutRqBulkCurrent();
};

#endif /* SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQBULKCURRENT_H_ */
