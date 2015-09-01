/*
 * PMVolumeClassic.h
 *
 *  Created on: Aug 27, 2015
 *      Author: luis
 */

#ifndef SRC_COMMON_PHYSICALMODEL_PMVOLUMECLASSIC_H_
#define SRC_COMMON_PHYSICALMODEL_PMVOLUMECLASSIC_H_

#include "PMVolume.h"

class PMVolumeClassic : public PMVolume {
public:
    PMVolumeClassic(
            const MatId matId,
            const string& name,
            const Real relativePermittivity = 1.0,
            const Real relativePermeability = 1.0,
            const Real electricConductivity = 0.0,
            const Real magneticConductivity = 0.0);
    virtual ~PMVolumeClassic();

    DEFINE_CLONE(PMVolumeClassic);

    Real getRelativePermittivity() const;
    Real getPermittivity() const;
    Real getRelativePermeability() const;
    Real getPermeability() const;
    Real getImpedance() const;
    Real getAdmitance() const;
    Real getElectricConductivity() const;
    Real getMagneticConductivity() const;
    bool isVacuum() const;
private:
    Real rEps_; // Rel. permittivity @ infte. freq.
    Real rMu_; // Rel. permeability @ infte. freq.
    Real electricConductivity_;
    Real magneticConudctivity_;
};

#endif /* SRC_COMMON_PHYSICALMODEL_PMVOLUMECLASSIC_H_ */
