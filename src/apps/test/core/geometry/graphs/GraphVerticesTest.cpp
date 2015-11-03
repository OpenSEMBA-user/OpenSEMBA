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
#include "geometry/graphs/GraphVertices.h"
#include "geometry/elements/GroupElements.h"

class GeometryGraphVerticesTest : public ::testing::Test {
    void SetUp() {
        cG_.add(new CoordR3(CoordinateId(1), CVecR3( 0.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordinateId(2), CVecR3( 1.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordinateId(3), CVecR3( 0.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordinateId(4), CVecR3( 1.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordinateId(5), CVecR3(-1.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordinateId(6), CVecR3(-1.0, 0.0, 0.0)));
        {
            CoordinateId vId[3] = {
                    CoordinateId(1), CoordinateId(2), CoordinateId(3)};
            elem_.add(new Tri3(cG_, ElementId(1), vId));
        }
        {
            CoordinateId vId[3] = {
                    CoordinateId(2), CoordinateId(4), CoordinateId(3)};
            elem_.add(new Tri3(cG_, ElementId(2), vId));
        }
        {
            CoordinateId vId[3] = {
                    CoordinateId(1), CoordinateId(3), CoordinateId(5)};
            elem_.add(new Tri3(cG_, ElementId(3), vId));
        }
        {
            CoordinateId vId[3] = {
                    CoordinateId(1), CoordinateId(5), CoordinateId(6)};
            elem_.add(new Tri3(cG_, ElementId(4), vId));
        }
        {
            CoordinateId vId[2] = {CoordinateId(1), CoordinateId(3)};
            elem_.add(new LinR2(cG_, ElementId(5), vId));
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

TEST_F(GeometryGraphVerticesTest, Ctor) {
    //  Creates Graph.
    GraphVertices<ElemR,CoordR3> graph;
    graph.init(elem_);
    EXPECT_EQ(5, graph.numElems());
    EXPECT_EQ(6, graph.numBounds());
}
