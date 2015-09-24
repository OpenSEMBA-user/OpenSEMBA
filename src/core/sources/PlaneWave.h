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
 * PlaneWave.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef PLANEWAVE_H_
#define PLANEWAVE_H_

#include "../sources/EMSource.h"

class PlaneWave : public EMSource<Vol> {
public:
    class ErrorZeroPolarization : public Error {
    public:
        ErrorZeroPolarization();
        virtual ~ErrorZeroPolarization() throw();
    };
    class ErrorZeroMagnitude : public Error {
    public:
        ErrorZeroMagnitude();
        virtual ~ErrorZeroMagnitude() throw();
    };
    class ErrorNotPerpendicular : public Error {
    public:
        ErrorNotPerpendicular();
        virtual ~ErrorNotPerpendicular() throw();
    };

    PlaneWave();
    PlaneWave(Magnitude* magnitude,
              GroupElements<Vol> elem,
              CVecR3 direction,
              CVecR3 polarization);
    PlaneWave(const PlaneWave& rhs);
    virtual ~PlaneWave();

    DEFINE_CLONE(PlaneWave);

    bool hasSameProperties(const EMSourceBase& rhs) const;

    const string& getName() const;
    const CVecR3& getPolarization() const;
    const CVecR3& getWaveDirection() const;
    Real getTheta() const;
    Real getPhi() const;
    Real getAlpha() const;
    Real getBeta() const;
    CVecR3 getElectricField(const Real time) const;
    pair<CVecR3,CVecR3> getElectromagneticField(const Real time) const;

    void printInfo() const;

private:
    CVecR3 direction_;
    CVecR3 polarization_;

    pair<Real,Real> cartesianToPolar(const CVecR3& vec) const;
    Real reduceRadians(const Real radianIn) const;
};

#endif /* PLANEWAVE_H_ */
