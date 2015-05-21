/*
 * CoordIntFractional.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

#ifndef  COORINFRACTIONAL_H_
# define COORINFRACTIONAL_H_

using namespace std;
#include "geometry/coordinates/Coordinate.h"
#include "geometry/elements/Element.h"
//#include "Pixel.h"

class CVecI3Fractional : public CVecI3 {

public:
    CVecI3Fractional ();
    CVecI3Fractional (const CoordI3 & _coordI, const CVecR3 &_len):
        CoordI3(_coordI), len_(_len){applyTol();}
    ~CVecI3Fractional (){}

    CVecI3Fractional& operator= (const CVecI3Fractional& rhs);

    bool less_coordI (const CVecI3Fractional &rhs) const;
    bool less (const CVecI3Fractional &rhs,
            const Real tool = MathUtils::tolerance) const;

    bool cmp_coordI (const CVecI3Fractional &rhs) const;
    bool cmp       (const CVecI3Fractional &rhs,
            const Real tool = MathUtils::tolerance) const;

    void applyTol (const Real tol=1.0e-4);

    CVecR3 getRelativePos () const;
    //bool IsInto(const Pixel& pixel) const;

    CVecI3 DiscretePositionDistribution (CVecI3 &numDivision) const;

    UInt getDirBase   ();
    UInt getRangeBase ();

protected:
    CVecR3 len_;
};

typedef CVecI3Fractional CVecI3Frac;

#endif
