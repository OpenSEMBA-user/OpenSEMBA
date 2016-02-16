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
#include "geometry/graph/Connectivities.h"
#include "geometry/element/Tetrahedron4.h"

using namespace SEMBA;
using namespace Geometry;
using namespace Math;

class GeometryConnectivitiesTest : public ::testing::Test {
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

TEST_F(GeometryConnectivitiesTest, ConnectivtiesReciprocity) {
    //  Creates reciprocal connectivities.
    {
        Graph::Connectivities conn(elem_);
        EXPECT_EQ(6, conn.size());
        EXPECT_TRUE(conn.existsReciprocity());
    }
    // Creates non recicprocal connectivities.
    const CoordR3* v[4] = {
            cG_.getId(CoordId(1)), cG_.getId(CoordId(2)),
            cG_.getId(CoordId(4)), cG_.getId(CoordId(5))};
    elem_.add(new Tet4(ElemId(7), v));
    {
        Graph::Connectivities conn(elem_);
        EXPECT_EQ(7, conn.size());
        EXPECT_FALSE(conn.existsReciprocity());
    }

}

TEST_F(GeometryConnectivitiesTest, InnerOuterFace) {
    Graph::Connectivities conn(elem_);
    {
        const Tri3* tri = elem_.getId(ElemId(6))->castTo<Tri3>();
        EXPECT_EQ(Element::Face(NULL,0), conn.getOuterFace(tri));
        EXPECT_TRUE(conn.isDomainBoundary(conn.getInnerFace(tri)));
    }

    {
        const Tri3* tri = elem_.getId(ElemId(5))->castTo<Tri3>();

        const Tet4* innerTet = elem_.getId(ElemId(3))->castTo<Tet4>();
        EXPECT_EQ(innerTet, conn.getInnerFace(tri).first);

        const Tet4* outerTet = elem_.getId(ElemId(4))->castTo<Tet4>();
        EXPECT_EQ(outerTet, conn.getOuterFace(tri).first);
    }
}

