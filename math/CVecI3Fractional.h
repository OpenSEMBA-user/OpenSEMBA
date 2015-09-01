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

typedef enum {
    dirNode = 0,
    dirX    = 1,
    dirY    = 2,
    dirZ    = 3
} CartesianDirection;

class CVecI3Fractional : public CVecI3 {
public:


    CVecI3Fractional ();
    CVecI3Fractional (const CVecI3& pos, const CVecR3& len):
        CVecI3(pos), len_(len){}
    ~CVecI3Fractional (){}

    CVecI3Fractional& operator= (const CVecI3Fractional& rhs);

    bool operator==(const CVecI3Fractional &rhs) const;

    CVecR3 getScalePos() const;
    CVecR3 getScalePos(const CVecI3 origin) const;

    CVecR3 meanRelativePoint (const CVecI3Fractional& rhs) const;
    CVecR3 getRelativePosNearestNode () const;
    CVecI3Fractional& reduceCoords();
    CVecI3Fractional& reduceTopology();
    CVecI3Fractional& reduceTopology(const UInt range);

    void join(CVecI3Fractional& rhs);
    CVecI3Fractional& move(
            CVecI3Fractional& rhs,
            const bool forceProject,
            bool& canBeMoved) const;

    CartesianDirection getDirBase   () const;
    UInt getRangeBase () const;

    string toStr() const;

    friend std::ostream& operator<<(ostream& os, const CVecI3Fractional& vec) {
       return os << vec.toStr();
    }
    CVecR3 len_;
};

typedef CVecI3Fractional CVecI3Frac;
//typedef CVecI3Fractional::Direction CartesianDirection;

#endif
