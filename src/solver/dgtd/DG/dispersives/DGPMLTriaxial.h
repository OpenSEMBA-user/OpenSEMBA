/*
 * DGPMLTriaxial.h
 *
 *  Created on: Jun 22, 2015
 *      Author: luis
 */

#ifndef SRC_SOLVER_DGTD_DG_DISPERSIVES_DGPMLTRIAXIAL_H_
#define SRC_SOLVER_DGTD_DG_DISPERSIVES_DGPMLTRIAXIAL_H_

#include "DGPMLMultiaxial.h"

class DGPMLTriaxial: public DGPMLMultiaxial {
public:
    DGPMLTriaxial(
            const PMVolumePML& mat,
            const CellGroup& cells,
            const bool useConductivity,
            const Real conductivity);
    virtual ~DGPMLTriaxial();
    void computeRHSElectric(
            FieldR3& rhs,
            const FieldR3& f,
            const UInt e1, const UInt e2) const;
    void computeRHSMagnetic(
            FieldR3& rhs,
            const FieldR3& f,
            const UInt e1, const UInt e2) const;
    void computeRHSElectricPolarizationCurrents(
            const FieldR3& f,
            const UInt e1, const UInt e2);
    void computeRHSMagneticPolarizationCurrents(
            const FieldR3& f,
            const UInt e1, const UInt e2);
};

typedef DGPMLTriaxial DGPMLxyz;

#endif /* SRC_SOLVER_DGTD_DG_DISPERSIVES_DGPMLTRIAXIAL_H_ */
