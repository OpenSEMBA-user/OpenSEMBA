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
#ifndef SRC_TEST_COORDINATEGROUPTEST_H_
#define SRC_TEST_COORDINATEGROUPTEST_H_

#include "gtest/gtest.h"
#include "geometry/coordinate/Group.h"

class GeometryCoordinateGroupTest : public ::testing::Test {

protected:
    std::vector<SEMBA::Geometry::CoordR3*> newCoordR3Vector() {
        SEMBA::Geometry::CoordId id(1);
        SEMBA::Math::CVecR3 one(1.0, 1.0, 1.0);
        const size_t nCoords = 5;
        std::vector<SEMBA::Geometry::CoordR3*> res;
        for (size_t i = 0; i < nCoords; i++) {
            res.push_back(new SEMBA::Geometry::CoordR3(id++, one * (double) i));
        }
        return res;
    }

    bool checkTypes(const SEMBA::Geometry::Coordinate::Group<>& rhs) {
        bool res = true;
        for (size_t i = 0; i < rhs.size(); i++) {
            res &= rhs(i)->is<SEMBA::Geometry::Coord>();
        }
        return res;
    }
};

#endif
