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
    CVecI3Fractional (const CVecI3& pos, const CVecR3& len):
        CVecI3(pos), len_(len){}
    ~CVecI3Fractional (){}

    CVecI3Fractional& operator= (const CVecI3Fractional& rhs);

    bool operator==(const CVecI3Fractional &rhs) const;
    bool less(const CVecI3Fractional &rhs,
            const Real tool = MathUtils::tolerance) const;


    CVecR3 getScalePos() const;
    CVecR3 getScalePos(const CVecI3 origin) const;

    CVecR3 meanRelativePoint (const CVecI3Fractional& rhs) const;
    CVecR3 getRelativePosNearestNode () const;
    void reduceCoords();
    void reduceTopology();
    void joinGeom(CVecI3Fractional& rhs);
    CVecI3Fractional& move(
            CVecI3Fractional& rhs,
            const bool forceProject,
            bool& canBeMoved) const;

    CVecI3 getBasePos () const{return *this->CVecI3::val;}
    CVecI3 DiscretePositionDistribution (const CVecI3 &numDivision) const;
    CVecI3 DiscretePositionDistribution (const CVecI3 &numDivision,
                                         const CVecI3 &origin) const;

    Direction getDirBase   () const;
    UInt getRangeBase () const;

    string toStr() const;

    friend std::ostream& operator<<(ostream& os, const CVecI3Fractional& vec) {
       return os << vec.toStr();
    }
//TODO
//private:
    CVecR3 len_;
};

typedef CVecI3Fractional CVecI3Frac;
typedef CVecI3Fractional::Direction CartesianDirection;

#endif
