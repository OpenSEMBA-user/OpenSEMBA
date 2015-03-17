/*
 * OutRqVolume.h
 *
 *  Created on: Mar 17, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQVOLUME_H_
#define SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQVOLUME_H_

#include "OutRq.h"

class OutRqVolume: public OutRq, public ElementsGroup<VolR> {
public:
    OutRqVolume();
    virtual ~OutRqVolume();
};

#endif /* SRC_COMMON_INPUTS_OUTPUTREQUEST_OUTRQVOLUME_H_ */
