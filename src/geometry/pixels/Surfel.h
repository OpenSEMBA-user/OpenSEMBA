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
/*
 * Surfel.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz-Cabello N.
 */

#ifndef SRC_MESHER_FDTD_MESHCONF_SURFEL_H_
#define SRC_MESHER_FDTD_MESHCONF_SURFEL_H_

#include "Linel.h"

typedef array<Linel,4> SurfelLinels;
typedef array<Pixel,4> SurfelPixels;

class Surfel: public Pixel {
public:
    Surfel(){normId_ = CartesianDirection::dirNode;};
    Surfel(const CVecI3& pos): Pixel(pos){normId_ = CartesianDirection::dirNode;}
    Surfel(const CVecI3& pos, const CartesianDirection& norm) : Pixel(pos), normId_(norm){};

    virtual ~Surfel(){};

    bool isInto (const CVecI3Frac& intFractional)const;

    bool operator==(const Surfel& rhs) const;
    bool operator<(const Surfel& rhs) const;

    CartesianDirection  getNormId ()const{return normId_;}
    CartesianDirection& NormId (){return normId_;}
    void setNormId (const CartesianDirection& _normId){normId_ = _normId;}

//    SurfelLinels getLinels() const;
//    Linel getLinel(const UInt side) const;
    SurfelPixels getPixels() const;

    string toStr() const;
    friend std::ostream& operator<<(ostream& os, const Surfel& vec) {
       return os << vec.toStr();
    }

protected:
    CartesianDirection normId_;
};

#endif /* SRC_MESHER_FDTD_MESHCONF_SURFEL_H_ */
