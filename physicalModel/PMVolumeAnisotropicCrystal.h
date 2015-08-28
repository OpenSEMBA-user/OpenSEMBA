/*
 * PMVolumeAnisotropicCrystal.h
 *
 *  Created on: Aug 28, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICCRYSTAL_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICCRYSTAL_H_

#include "PMVolumeAnisotropic.h"

// Described in: https://courses.cit.cornell.edu/ece303/Lectures/lecture17.pdf
class PMVolumeAnisotropicCrystal: public PMVolumeAnisotropic {
public:
    PMVolumeAnisotropicCrystal(
            const MatId matId,
            const string& name,
            const LocalAxes& local,
            const CVecR3& principalAxesRelativePermittivity,
            const Real relativePermeability);
    virtual ~PMVolumeAnisotropicCrystal();

    const CVecR3 getPrincipalAxesRelativePermittivity() const;
    Real getRelativePermeability() const;

    MatR33 getRelPermittivityMatR() const;
    MatR33 getRelPermeabilityMatR() const;
    MatR33 getElectricConductivityMat() const;
    MatR33 getMagneticConductivityMat() const;
private:
    CVecR3 principalAxesRelativePermittivity_;
    Real relativePermeability_;
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMEANISOTROPICCRYSTAL_H_ */
