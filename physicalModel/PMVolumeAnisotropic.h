/*
 * PMVolumeAnisotropic.h
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_

#include "PMVolume.h"

class PMVolumeAnisotropic: public PMVolume {
public:
    PMVolumeAnisotropic(
            const MatId matId,
            const string& name);
    virtual ~PMVolumeAnisotropic();
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_ */
