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

#ifndef SEMBA_GEOMETRY_PIXEL_SURFEL_H_
#define SEMBA_GEOMETRY_PIXEL_SURFEL_H_

#include <array>

#include "Linel.h"

namespace SEMBA {
namespace Geometry {
namespace Pixel {

typedef std::array<Linel,4> SurfelLinels;
typedef std::array<Pixel,4> SurfelPixels;

class Surfel: public Pixel {
public:
    Surfel(){normId_ = Math::CartesianDirection::dirNode;};
    Surfel(const Math::CVecI3& pos)
    :   Pixel(pos){normId_ = Math::CartesianDirection::dirNode;}
    Surfel(const Math::CVecI3& pos, const Math::CartesianDirection& norm)
    :   Pixel(pos), normId_(norm){};

    virtual ~Surfel(){};

    bool isInto (const Math::CVecI3Frac& intFractional)const;

    bool operator==(const Surfel& rhs) const;
    bool operator<(const Surfel& rhs) const;

    Math::CartesianDirection  getNormId ()const{return normId_;}
    Math::CartesianDirection& NormId (){return normId_;}
    void setNormId (const Math::CartesianDirection& _normId){normId_ = _normId;}

//    SurfelLinels getLinels() const;
//    Linel getLinel(const UInt side) const;
    SurfelPixels getPixels() const;

    std::string toStr() const;
    friend std::ostream& operator<<(std::ostream& os, const Surfel& vec) {
       return os << vec.toStr();
    }

protected:
    Math::CartesianDirection normId_;
};

} /* namespace Pixel */
} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_PIXEL_SURFEL_H_ */
