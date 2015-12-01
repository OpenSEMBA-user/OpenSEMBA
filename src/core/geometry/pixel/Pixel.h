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

#ifndef SEMBA_GEOMETRY_PIXEL_PIXEL_H_
#define SEMBA_GEOMETRY_PIXEL_PIXEL_H_

#include "Types.h"
#include "math/vector/CVecI3Fractional.h"

namespace SEMBA {
namespace Geometry {
namespace Pixel {

class Pixel : public Math::CVecI3 {
public:
    Pixel() : Math::CVecI3() {}
    Pixel(const Math::CVecI3& cVecI3_) : Math::CVecI3(cVecI3_) {}
    virtual ~Pixel() {}

    bool operator<(const Pixel& rhs) const;

    void printInfo() const;

    static std::string toStr(const Math::CartesianDirection& dir);
};

} /* namespace Pixel */
} /* namespace Geometry */
} /* namespace SEMBA */

#endif /* SEMBA_GEOMETRY_PIXEL_PIXEL_H_ */
