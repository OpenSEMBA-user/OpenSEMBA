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

#ifndef SRC_APPS_TEST_CORE_GEOMETRY_GRAPH_CONNECTIVITIESTEST_H_
#define SRC_APPS_TEST_CORE_GEOMETRY_GRAPH_CONNECTIVITIESTEST_H_

#include "gtest/gtest.h"
#include "geometry/graph/Connectivities.h"
#include "geometry/element/Tetrahedron4.h"

using namespace SEMBA;
using namespace Math;
using namespace Geometry;

class GeometryGraphConnectivitiesTest : public ::testing::Test {
    void SetUp() {
        cG_.add(new CoordR3(CoordId(1), CVecR3( 0.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordId(2), CVecR3( 0.0, 0.0, 1.0)));
        cG_.add(new CoordR3(CoordId(3), CVecR3( 1.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordId(4), CVecR3( 0.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordId(5), CVecR3(-1.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordId(6), CVecR3( 1.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordId(7), CVecR3( 1.0, 1.0, 1.0)));
        cG_.add(new CoordR3(CoordId(8), CVecR3( 2.0, 0.0, 0.0)));
        {
            const CoordR3* v[4] = {
                    cG_.getId(CoordId(1)), cG_.getId(CoordId(2)),
                    cG_.getId(CoordId(3)), cG_.getId(CoordId(4))};
            elem_.add(new Tet4(ElemId(1), v));
        }
        {
            const CoordR3* v[4] = {
                    cG_.getId(CoordId(1)), cG_.getId(CoordId(2)),
                    cG_.getId(CoordId(4)), cG_.getId(CoordId(5))};
            elem_.add(new Tet4(ElemId(2), v));
        }
        {
            const CoordR3* v[4] = {
                    cG_.getId(CoordId(3)), cG_.getId(CoordId(4)),
                    cG_.getId(CoordId(6)), cG_.getId(CoordId(7))};
            elem_.add(new Tet4(ElemId(3), v));
        }
        {
            const CoordR3* v[4] = {
                    cG_.getId(CoordId(3)), cG_.getId(CoordId(8)),
                    cG_.getId(CoordId(6)), cG_.getId(CoordId(7))};
            elem_.add(new Tet4(ElemId(4), v));
        }
        {
            const CoordR3* v[3] = {
                    cG_.getId(CoordId(3)),  cG_.getId(CoordId(6)),
                    cG_.getId(CoordId(7))};
            elem_.add(new Tri3(ElemId(5), v));
        }
        {
            const CoordR3* v[3] = {
                    cG_.getId(CoordId(1)),  cG_.getId(CoordId(2)),
                    cG_.getId(CoordId(3))};
            elem_.add(new Tri3(ElemId(6), v));
        }
    }
    void TearDown() {
        cG_.clear();
        elem_.clear();
    }
protected:
    CoordR3Group cG_;
    ElemRGroup elem_;
};


#endif /* SRC_APPS_TEST_CORE_GEOMETRY_GRAPH_CONNECTIVITIESTEST_H_ */
