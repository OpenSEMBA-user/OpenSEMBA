// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
/*
 * SolverPlaneWave.h
 *
 *  Created on: Sep 11, 2012
 *      Author: luis
 */

#ifndef SOLVERPLANEWAVE_H_
#define SOLVERPLANEWAVE_H_

#include "DGSource.h"
#include "sources/PlaneWave.h"

using namespace std;

class DGPlaneWave : public DGSource, public PlaneWave {
public:
    DGPlaneWave(
            const PlaneWave& pw,
            const Comm* comm,
            FieldR3& dE, FieldR3& dH,
            const Int vmapM[faces][nfp]);
    virtual ~DGPlaneWave();
    void computeExcitation(
            const Real intTime,
            const Real minDT);
    void printInfo() const;
private:
    Real *kNPosTF; // dim(nETSF*SOLVER_NFP)
    Real *kNPosSF; // dim(nETSF*SOLVER_NFP)
    Real *kNPosTFNB; // dim(nETFNB*SOLVER_NFP)
    void computeExcitationField(
            Real* ExInc, Real *EyInc, Real *EzInc,
            Real* HxInc, Real *HyInc, Real *HzInc,
            const Real* vPos,
            const UInt nE,
            const Real intTime);
    void initWaveNumberPosition(
            const Comm* comm,
            const Int vmapM[faces][nfp]);
};

#endif /* SOLVERPLANEWAVE_H_ */
