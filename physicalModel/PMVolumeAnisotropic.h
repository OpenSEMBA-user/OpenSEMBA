/*
 * PMVolumeAnisotropic.h
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_

#include "PMVolume.h"
#ibclude "math/LocalAxes.h"

class PMVolumeAnisotropic: public PMVolume {
public:
    PMVolumeAnisotropic(
            const MatId matId,
            const string& name,
            const LocalAxes& local,
            const array<PMVolume,3> tensor
            );
    virtual ~PMVolumeAnisotropic();
    MatR33 getRelPermittivityMatrix() const;
    MatR33 getRelPermeabilityMatrix() const;
    MatR33 getElectricConductivity() const;
    MatR33 getMagneticConductivity() const;
private:
    LocalAxes localAxe_;
    array<PMVolume*,3> tensor_;
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_ */
