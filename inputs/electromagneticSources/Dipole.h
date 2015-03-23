/*
 * Dipole.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef DIPOLE_H_
#define DIPOLE_H_

#include "EMSource.h"

class Dipole : public EMSource<Vol> {
public:
    Dipole(const MagnitudeGaussian* magnitude,
           const ElementsGroup<Vol>& elem,
           Real   length,
           CVecR3 orientation,
           CVecR3 position);
    Dipole(const Dipole& rhs);
    virtual ~Dipole();

    virtual ClassBase* clone() const;

    void printInfo() const;
protected:
    Real length_;
    CVecR3 orientation_;
    CVecR3 position_;
    Real gaussDelay_;
    Real spreadSqrt2_;
};

#endif /* DIPOLE_H_ */
