/*
 * CoordIntFractional.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

#ifndef  COORINFRACTIONAL_H_
# define COORINFRACTIONAL_H_

using namespace std;
#include "math/CartesianVector.h"
#include "geometry/coordinates/Coordinate.h"
#include "geometry/elements/Element.h"

class CVecI3Fractional : public CVecI3 {

public:
    CVecI3Fractional ();
    CVecI3Fractional (const CVecI3 & _cvecI3, const CVecR3 &_len):
        CVecI3(_cvecI3), len_(_len){applyTol();}
    ~CVecI3Fractional (){}

    CVecI3Fractional& operator= (const CVecI3Fractional& rhs);

    bool less(const CVecI3Fractional &rhs,
            const Real tool = MathUtils::tolerance) const;

    void applyTol (const Real tol=1.0e-4);

    CVecR3 getRelativePos () const;

    CVecI3 DiscretePositionDistribution (CVecI3 &numDivision) const;

    CartesianDirection getDirBase   () const;
    UInt getRangeBase () const;

private:
    CVecR3 len_;
};

typedef CVecI3Fractional CVecI3Frac;

#endif
