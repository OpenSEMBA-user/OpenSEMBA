

#pragma once

#include "Cartesian.h"

namespace SEMBA {
namespace Math {

enum CartesianDirection {
    dirNode = 0,
    dirX    = 1,
    dirY    = 2,
    dirZ    = 3
};

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
    CVecI3Fractional& reduceTopology(const std::size_t range);

    void join(CVecI3Fractional& rhs);
    CVecI3Fractional& move(CVecI3Fractional& rhs,
                           const bool forceProject,
                           bool& canBeMoved) const;

    CartesianDirection getDirBase   () const;
    std::size_t getRangeBase() const;

    std::string toStr() const;

    friend std::ostream& operator<<(std::ostream& os,
                                    const CVecI3Fractional& vec) {
        return os << vec.toStr();
    }
    CVecR3 len_;
};

typedef CVecI3Fractional CVecI3Frac;

} /* namespace Math */
} /* namespace SEMBA */

