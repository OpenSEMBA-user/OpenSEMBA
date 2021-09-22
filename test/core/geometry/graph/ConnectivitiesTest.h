

#pragma once

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


