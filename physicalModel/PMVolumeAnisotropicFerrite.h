/*
 * PMVolumeAnistropicFerrite.h
 *
 *  Created on: Aug 28, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICFERRITE_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICFERRITE_H_

#include "PMVolumeAnisotropic.h"

// Described using Polder tensor model:
// https://en.wikipedia.org/wiki/Polder_tensor
class PMVolumeAnisotropicFerrite: public PMVolumeAnisotropic {
public:
    PMVolumeAnisotropicFerrite(
            const MatId matId,
            const string& name,
            const LocalAxes& local,
            const Real kappa,
            const Real relativePermeability,
            const Real relativePermittivity);
    virtual ~PMVolumeAnisotropicFerrite();

    MatR33 getRelPermittivityMatR() const;

    MatR33 getRelPermeabilityMatR() const; // Real part.
    MatR33 getRelPermeabilityMatI() const; // Imaginary part.

    MatR33 getElectricConductivityMat() const;
    MatR33 getMagneticConductivityMat() const;
private:
    Real kappa_;
    Real relativePermeability_;
    Real relativePermittivity_;
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICFERRITE_H_ */
