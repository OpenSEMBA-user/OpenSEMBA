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
    PlaneWave(const Magnitude* magnitude,
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
