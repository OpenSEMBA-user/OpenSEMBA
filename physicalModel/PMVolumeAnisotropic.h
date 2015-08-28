/*
 * PMVolumeAnisotropic.h
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_

#include "PMVolume.h"
#include "math/LocalAxes.h"

class PMVolumeAnisotropic: public PMVolume {
public:
    PMVolumeAnisotropic(
            const MatId matId,
            const string& name,
            const LocalAxes& local);
    virtual ~PMVolumeAnisotropic();

    LocalAxes getLocalAxe() const;
    virtual MatR33 getRelPermittivityMatR() const = 0;
    virtual MatR33 getRelPermeabilityMatR() const = 0;
    virtual MatR33 getElectricConductivityMat() const = 0;
    virtual MatR33 getMagneticConductivityMat() const = 0;

private:
    LocalAxes localAxe_;
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPIC_H_ */
