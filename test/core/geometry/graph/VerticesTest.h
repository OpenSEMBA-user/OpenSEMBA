

#pragma once

#include "gtest/gtest.h"
#include "geometry/graph/Vertices.h"
#include "geometry/element/Group.h"
#include "geometry/element/Triangle3.h"
#include "geometry/element/Line2.h"

using namespace SEMBA;
using namespace Math;
using namespace Geometry;

class GeometryGraphVerticesTest : public ::testing::Test {
    void SetUp() {
        cG_.add(new CoordR3(CoordId(1), CVecR3( 0.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordId(2), CVecR3( 1.0, 0.0, 0.0)));
        cG_.add(new CoordR3(CoordId(3), CVecR3( 0.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordId(4), CVecR3( 1.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordId(5), CVecR3(-1.0, 1.0, 0.0)));
        cG_.add(new CoordR3(CoordId(6), CVecR3(-1.0, 0.0, 0.0)));
        {
            const CoordR3* v[3] = {
                    cG_.getId(CoordId(1)), cG_.getId(CoordId(2)),
                    cG_.getId(CoordId(3))};
            elem_.add(new Tri3(ElemId(1), v));
        }
        {
            const CoordR3* v[3] = {
                    cG_.getId(CoordId(2)), cG_.getId(CoordId(4)),
                    cG_.getId(CoordId(3))};
            elem_.add(new Tri3(ElemId(2), v));
        }
        {
            const CoordR3* v[3] = {
                    cG_.getId(CoordId(1)), cG_.getId(CoordId(3)),
                    cG_.getId(CoordId(5))};
            elem_.add(new Tri3(ElemId(3), v));
        }
        {
            const CoordR3* v[3] = {
                    cG_.getId(CoordId(1)), cG_.getId(CoordId(5)),
                    cG_.getId(CoordId(6))};
            elem_.add(new Tri3(ElemId(4), v));
        }
        {
            const CoordR3* v[2] = {
                    cG_.getId(CoordId(1)), cG_.getId(CoordId(3))};
            elem_.add(new LinR2(ElemId(5), v));
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



