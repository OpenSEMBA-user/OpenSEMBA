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
#include "gtest/gtest.h"
#include "geometry/graphs/Connectivities.h"

class GeometryConnectivitiesTest : public ::testing::Test {
    void SetUp() {
        cG_.add(new CoordR3(CoordinateId(1), CVecR3( 0.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordinateId(2), CVecR3( 0.0, 0.0, 1.0)));
        cG_.add(new CoordR3(CoordinateId(3), CVecR3( 1.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordinateId(4), CVecR3( 0.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordinateId(5), CVecR3(-1.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordinateId(6), CVecR3( 1.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordinateId(7), CVecR3( 1.0, 1.0, 1.0)));
        cG_.add(new CoordR3(CoordinateId(8), CVecR3( 2.0, 0.0, 0.0)));
        LayerId lId(1);
        MatId mId(1);
        {
            CoordinateId vId[4] = {CoordinateId(1), CoordinateId(2),
                                   CoordinateId(3), CoordinateId(4)};
            elem_.add(new Tet4(cG_, ElementId(1), vId, lId, mId));
        }
        {
            CoordinateId vId[4] = {CoordinateId(1), CoordinateId(2),
                                   CoordinateId(4), CoordinateId(5)};
            elem_.add(new Tet4(cG_, ElementId(2), vId, lId, mId));
        }
        {
            CoordinateId vId[4] = {CoordinateId(3), CoordinateId(4),
                                   CoordinateId(6), CoordinateId(7)};
            elem_.add(new Tet4(cG_, ElementId(3), vId, lId, mId));
        }
        {
            CoordinateId vId[4] = {CoordinateId(3), CoordinateId(8),
                                   CoordinateId(6), CoordinateId(7)};
            elem_.add(new Tet4(cG_, ElementId(4), vId, lId, mId));
        }
        {
            CoordinateId vId[3] = {
                    CoordinateId(3),  CoordinateId(6), CoordinateId(7)};
            elem_.add(new Tri3(cG_, ElementId(5), vId, lId, mId));
        }
        {
            CoordinateId vId[3] = {
                    CoordinateId(1),  CoordinateId(2), CoordinateId(3)};
            elem_.add(new Tri3(cG_, ElementId(6), vId, lId, mId));
        }
    }
    void TearDown() {
        cG_.clear();
        elem_.clear();
    }
protected:
    CoordR3Group cG_;
    GroupElements<const ElemR> elem_;
};

TEST_F(GeometryConnectivitiesTest, GraphReciprocity) {
    //  Creates reciprocal connectivities.
    {
        Connectivities conn(elem_);
        EXPECT_EQ(6, conn.size());
        EXPECT_TRUE(conn.existsReciprocity());
    }
    // Creates non recicprocal connectivities.
    array<CoordinateId,4> vId = {CoordinateId(1), CoordinateId(2),
                           CoordinateId(4), CoordinateId(5)};
    elem_.add(new Tet4(cG_, ElementId(7), vId.begin(), LayerId(1), MatId(1)));
    {
        Connectivities conn(elem_);
        EXPECT_EQ(7, conn.size());
        EXPECT_FALSE(conn.existsReciprocity());
    }

}

