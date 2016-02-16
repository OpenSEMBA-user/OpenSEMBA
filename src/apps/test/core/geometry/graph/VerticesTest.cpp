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
#include "geometry/graph/Vertices.h"
#include "geometry/element/Group.h"
#include "geometry/element/Triangle3.h"

using namespace SEMBA;
using namespace Geometry;
using namespace Math;

class GeometryGraphVerticesTest : public ::testing::Test {
    void SetUp() {
        cG_.add(new CoordR3(CoordId(1), CVecR3( 0.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordId(2), CVecR3( 1.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordId(3), CVecR3( 0.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordId(4), CVecR3( 1.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordId(5), CVecR3(-1.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordId(6), CVecR3(-1.0, 0.0, 0.0)));
        {
            CoordId vId[3] = {CoordId(1), CoordId(2), CoordId(3)};
            elem_.add(new Tri3(cG_, ElemId(1), vId));
        }
//        {
//            CoordId vId[3] = {CoordId(2), CoordId(4), CoordId(3)};
//            elem_.add(new Tri3(cG_, ElemId(2), vId));
//        }
//        {
//            CoordId vId[3] = {CoordId(1), CoordId(3), CoordId(5)};
//            elem_.add(new Tri3(cG_, ElemId(3), vId));
//        }
//        {
//            CoordId vId[3] = {CoordId(1), CoordId(5), CoordId(6)};
//            elem_.add(new Tri3(cG_, ElemId(4), vId));
//        }
//        {
//            CoordId vId[2] = {CoordId(1), CoordId(3)};
//            elem_.add(new LinR2(cG_, ElemId(5), vId));
//        }
    }
    void TearDown() {
        cG_.clear();
        elem_.clear();
    }
protected:
    CoordR3Group cG_;
    ElemRGroup elem_;
};

TEST_F(GeometryGraphVerticesTest, Ctor) {
    //  Creates Graph.
    Graph::Vertices<ElemR,CoordR3> graph;
    graph.init(elem_);
    EXPECT_EQ(5, graph.numElems());
    EXPECT_EQ(6, graph.numBounds());
}
