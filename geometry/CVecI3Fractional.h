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


class CVecI3Fractional : public CVecI3 {
public:
    typedef enum {
        dirNode = 0,
        dirX    = 1,
        dirY    = 2,
        dirZ    = 3
    } Direction;
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

    Direction getDirBase   () const;
    UInt getRangeBase () const;

private:
    CVecR3 len_;
};

typedef CVecI3Fractional CVecI3Frac;
typedef CVecI3Fractional::Direction CartesianDirection;

#endif
