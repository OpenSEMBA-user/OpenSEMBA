/*
 * PMVolumeAnistropicFerrite.h
 *
 *  Created on: Aug 28, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISTROPICFERRITE_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISTROPICFERRITE_H_

#include "PMVolumeAnisotropic.h"

// Described using Polder tensor model:
// https://en.wikipedia.org/wiki/Polder_tensor
class PMVolumeAnistropicFerrite: public PMVolumeAnisotropic {
public:
    PMVolumeAnistropicFerrite();
    virtual ~PMVolumeAnistropicFerrite();
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISTROPICFERRITE_H_ */
