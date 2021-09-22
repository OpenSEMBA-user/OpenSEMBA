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

