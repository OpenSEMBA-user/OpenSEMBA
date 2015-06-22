/*
 * DGPMLBiaxial.h
 *
 *  Created on: Jun 22, 2015
 *      Author: luis
 */

#ifndef SRC_SOLVER_DGTD_DG_DISPERSIVES_DGPMLBIAXIAL_H_
#define SRC_SOLVER_DGTD_DG_DISPERSIVES_DGPMLBIAXIAL_H_

#include "DGPMLMultiaxial.h"

template<Int D>
class DGPMLBiaxial: public DGPMLMultiaxial {
public:
    DGPMLBiaxial(
            const PMVolumePML& mat_,
            const CellGroup& cells,
            const bool useConductivity,
            const Real conductivity););
    virtual ~DGPMLBiaxial();
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

#include "DGPMLBiaxial.hpp"

typedef DGPMLBiaxial<x> DGPMLxy;
typedef DGPMLBiaxial<y> DGPMLyz;
typedef DGPMLBiaxial<z> DGPMLzx;

#endif /* SRC_SOLVER_DGTD_DG_DISPERSIVES_DGPMLBIAXIAL_H_ */
