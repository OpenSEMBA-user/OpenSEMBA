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
#ifndef SRC_TEST_ELEMENTSGROUPTEST_H_
#define SRC_TEST_ELEMENTSGROUPTEST_H_

#include "gtest/gtest.h"
#include "geometry/element/Group.h"

using namespace std;

using namespace SEMBA;
using namespace Geometry;

class GeometryElementGroupTest : public ::testing::Test {

protected:
    bool checkTypes(const Element::Group<>& rhs) {
        bool res = true;
        for (size_t i = 0; i < rhs.size(); i++) {
            res &= rhs(i)->is<Elem>();
        }
        return res;
    }

    vector<CoordR3*> newCoordR3Vector() const {
        vector<CoordR3*> res;
        res.push_back(new CoordR3(CoordId(1), Math::CVecR3(1.0, 1.0, 1.0)));
        res.push_back(new CoordR3(CoordId(2), Math::CVecR3(2.0, 2.0, 2.0)));
        return res;
    }
};

#endif
