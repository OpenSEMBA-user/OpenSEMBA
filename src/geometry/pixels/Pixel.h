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
 * Pixel.h
 *
 *  Created on: May 19, 2015
 *      Author: Miguel D. Ruiz - Cabello Nuñez
 */

#ifndef SRC_MESHER_FDTD_MESHCONF_PIXEL_H_
#define SRC_MESHER_FDTD_MESHCONF_PIXEL_H_

#include "math/CVecI3Fractional.h"
#include "base/class/ClassBase.h"
#include <array>

class Pixel : public CVecI3 {
public:
    Pixel():CVecI3(){};
    Pixel(const CVecI3& cVecI3_): CVecI3(cVecI3_){};
    virtual ~Pixel(){};

    bool operator<(const Pixel& rhs) const;

    void printInfo() const;

    static string toStr(const CartesianDirection& dir);
};

#endif /* SRC_MESHER_FDTD_MESHCONF_PIXEL_H_ */
