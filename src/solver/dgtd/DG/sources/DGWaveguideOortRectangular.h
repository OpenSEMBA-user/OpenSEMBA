/*
 * SolverWaveportRectangular.h
 *
 *  Created on: Aug 26, 2013
 *      Author: luis
 */

#ifndef SOLVERWAVEPORTRECTANGULAR_H_
#define SOLVERWAVEPORTRECTANGULAR_H_

#include "../../../../common/sources/PortWaveguide.h"
#include "DGWaveport.h"

class DGWaveportRectangular : public DGWaveport, public PortWaveguide {
public:
    DGWaveportRectangular(
            const PortWaveguide& pw,
            const MapGroup& map,
            FieldR3& dE, FieldR3& dH,
            const Int vmapM[faces][nfp]);
    virtual ~DGWaveportRectangular();
    void computeExcitation(
            const Real intTime,
            const Real minDT);
    void printInfo() const;
private:
    Real width, height;
    PortWaveguide::ExcitationMode excitationMode;
    Real kcm;
    Real intrinsicImpedance;
    Real gammaMSum;
    void computeExcitationField(
            FieldR3& EInc,
            FieldR3& HInc,
            const CVecR3* pos,
            const UInt nE,
            const Real intTime,
            const Real minDT);
};

#endif /* SOLVERWAVEPORTRECTANGULAR_H_ */
