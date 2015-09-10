/*
 * Dipole.h
 *
 *  Created on: Jun 28, 2013
 *      Author: luis
 */

#ifndef DIPOLE_H_
#define DIPOLE_H_

#include "../sources/EMSource.h"

class Dipole : public EMSource<Vol> {
public:
    Dipole(const Magnitude& magnitude,
           const GroupElements<Vol>& elem,
           Real   length,
           CVecR3 orientation,
           CVecR3 position);
    Dipole(const Dipole& rhs);
    virtual ~Dipole();

    DEFINE_CLONE(Dipole);

    bool hasSameProperties(const EMSourceBase& rhs) const;

    const string& getName() const;
    void printInfo() const;
protected:
    Real length_;
    CVecR3 orientation_;
    CVecR3 position_;
    Real gaussDelay_;
    Real spreadSqrt2_;
};

#endif /* DIPOLE_H_ */
